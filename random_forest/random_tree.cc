#include "random_tree.h"
//#include "objective_function.h"
#include "forest_helper.h"

#include <deque>
#include <cassert>

namespace forest {

RandomTree::RandomTree(size_t tree_id, TreeParams& params,
                       std::shared_ptr<SplitSamplerBase>& split_sampler) :
    tree_id_(tree_id), params_(params), split_sampler_(split_sampler) {
  //objective_function_ = std::make_shared<InformationGain>();

  // randomly sample subset of training data
  train_subset_ = SampleTrainingSubset();
  //train_labels_ = split_sampler_->GetTrainLabels();
  //UpdateClassDistributions(data, train_subset_);
}

bool RandomTree::StopSplitting(const Node& node) {
  //assert(node.entropy == 0 || node.entropy > 0.0001);
  if (node.depth == params_.max_depth || node.samples.size() < params_.min_samples_split)
    return true;
  // Stop if the entropy is zero
  else if (node.entropy < 1e-5)
    return true;
  else
    return false;
}

//void RandomTree::UpdateClassDistributions(const DataBase& data,
//                                          const std::vector<size_t>& train_subset) {
//  class_distribution_full_ = data.GetClassDistribution();
//  class_distribution_subset_ = data.GetClassDistribution(train_subset);
//}

//bool RandomTree::Grow() {
bool RandomTree::Grow(const DataBase& data) {
  // Initialize node list with root
  root_ = std::make_shared<Node>(data, train_subset_);
  // TODO priority queue
  std::deque<std::shared_ptr<Node>> candidate_nodes;
  candidate_nodes.push_back(root_);

  //size_t iter = 0;
  while (candidate_nodes.size() > 0) {
    //iter++;
    //if (iter == 6)
    //  std::cout << "";
    //std::vector<std::shared_ptr<FeaturesAndThresholdsBase>> features;
    // Create random features and threshold candidates for each feature
    auto node = candidate_nodes.front();
    candidate_nodes.pop_front();
    if (StopSplitting(*node)) {
      node->DeclareLeaf(data);
      continue;
    }

    // Generate only valid thresholds
    //double best_loss = std::numeric_limits<double>::max();

    // TODO split types - hard/soft, uni/multi
    std::vector<size_t> left_split, right_split;
    std::shared_ptr<SplitBase> split = split_sampler_->FindBestSplit(params_, node->samples,
        left_split, right_split);
        //objective_function_, left_split, right_split, best_gain);
    // If none of the generated splits was good enough set the node as leaf
    //if (best_gain < params_.min_gain || !split_found) {
    if (!split) {
      node->DeclareLeaf(data);
      continue;
    }

    auto left_child = std::make_shared<Node>(data, node, left_split);
    auto right_child = std::make_shared<Node>(data, node, right_split);
    node->left = left_child;
    node->right = right_child;
    node->splitter_ = std::move(split);
    //node->feature_function = features[best_feature];
    //node->threshold = thresholds[best_feature][best_thr];
    // Release the vector containing indices in parent node
    node->samples.clear();
    node->samples.shrink_to_fit();
    candidate_nodes.push_back(left_child);
    candidate_nodes.push_back(right_child);
  }

  return true;
}

std::vector<std::shared_ptr<Node>> RandomTree::CollectSubtreeLeafs(
    const std::shared_ptr<Node> subtree) const {
  if (subtree->is_leaf)
    return { subtree };
  assert(subtree->left != nullptr && subtree->right != nullptr);
  //return { CollectLeafs(subtree->left), CollectLeafs(subtree->right) };
  std::vector<std::shared_ptr<Node>> left_leafs = CollectSubtreeLeafs(subtree->left);
  std::vector<std::shared_ptr<Node>> right_leafs = CollectSubtreeLeafs(subtree->right);
  left_leafs.insert(left_leafs.end(), right_leafs.begin(), right_leafs.end());
  return left_leafs;
}

std::vector<double> RandomTree::Predict(const DataBase& data, size_t sample_num) const {
  std::shared_ptr<Node> node_runner = root_;
  while (node_runner->is_leaf == false) {
    if (node_runner->Split(data, sample_num) == SplitType::kLeftSplit)
      node_runner = node_runner->left;
    else
      node_runner = node_runner->right;
  }
  assert(node_runner->class_distribution.size() > 0);
  return node_runner->class_distribution;
}

std::vector<size_t> RandomTree::SampleTrainingSubset() {
  size_t train_size = split_sampler_->TrainSize();
  size_t sample_size;
  if (params_.train_percentage <= 0.0) {
    throw 1;
    //sample_size = std::round(5.0 * train_size / params_.num_trees);
    //sample_size = std::min(sample_size, train_size);
  }
  else {
    sample_size = std::round(params_.train_percentage * train_size);
  }
  //std::vector<size_t> sample = ForestHelper::
  //    GenerateUniqueIntegers<size_t>(rng, sample_size, 0, train_size-1);
  std::vector<size_t> train_samples;
  if (sample_size == train_size) {
    train_samples.resize(sample_size);
    train_samples = std::vector<size_t>(sample_size);
    std::iota(train_samples.begin(), train_samples.end(), 0);
    //std::cout << train_samples << "\n";
  }
  else
    train_samples = ForestHelper::
      GenerateRandomIntegers<size_t>(*params_.rng, sample_size, 0, train_size-1);
  return train_samples;
}

//bool RandomTree::Grow(const DataBase& data, const std::vector<size_t>& train_subset) {
//  UpdateClassDistributions(data, train_subset);
//  // Initialize node list with root
//  root_ = std::make_shared<Node>(data, train_subset);
//  // priority queue
//  std::deque<std::shared_ptr<Node>> candidate_nodes;
//  candidate_nodes.push_back(root_);
//
//  //size_t iter = 0;
//  while (candidate_nodes.size() > 0) {
//    //iter++;
//    //if (iter == 6)
//    //  std::cout << "";
//    std::vector<std::shared_ptr<FilterBase>> features;
//    std::vector<std::vector<double>> thresholds;
//    // Create random features and threshold candidates for each feature
//    auto node = candidate_nodes.front();
//    candidate_nodes.pop_front();
//    if (StopSplitting(*node)) {
//      node->SetLeaf(true, data);
//      continue;
//    }
//    // Generate only valid thresholds
//    data.GenerateSplitCandidates(params_, node->samples, features, thresholds);
//    //double best_loss = std::numeric_limits<double>::max();
//    double best_gain = std::numeric_limits<double>::min();
//    std::vector<size_t> best_left_split, best_right_split;
//    size_t best_feature, best_thr;
//    bool split_success = false;
//    // TODO optimize cache - samples first!
//    //std::vector<CriteriaBase> criterias;
//    //splitter_->GenerateSplitCandidates(data, params_, node->samples, features, criterias);
//    //splitter_->GenerateSplitCandidates(data, params_, node->samples);
//    //split_generator_->Generate(data, params_, node->samples, splits);
//    //for (size_t i = 0; i < samples.size(); i++)
//    //  for (size_t j = 0; j < filters.size(); j++)
//    //    for (size_t k = 0; k < criterias.size(); k++)
//    //      if (splitter_->Split(data, i, filters[j], criterias[k]) == SplitType::LEFT)
//    //      if (splitter_->Split(data, i, j, k) == SplitType::LEFT) bad?
//    //      if (split[j]->Split...)
//
//    for (size_t i = 0; i < features.size(); i++) {
//      for (size_t j = 0; j < thresholds.size(); j++) {
//        std::vector<size_t> left_split, right_split;
//        if (!SplitData(data, *features[i], thresholds[i][j], node->samples, left_split, right_split))
//          continue;
//        //assert(!left_split.empty() && !right_split.empty());
//        // TODO Generate splitter classes instead raw thresholds?
//        //splitters[i][j]->Split(data, features[i], node->samples, left_split, right_split);
//
//        //double loss = loss_function_->Evaluate(data, left_split, right_split);
//        double gain = loss_function_->Evaluate(data, node->samples, left_split, right_split);
//        assert(gain >= -1e-10 && gain <= 2.0);
//        //std::cout << gain << "\n";
//        // If the split is the best so far, save it
//        //if (loss < best_loss) {
//        if (gain > best_gain && gain >= params_.min_gain) {
//          split_success = true;
//          //best_loss = loss;
//          best_gain = gain;
//          best_feature = i;
//          best_thr = j;
//          best_left_split = std::move(left_split);
//          best_right_split = std::move(right_split);
//        }
//      }
//    }
//    // If none of the generated splits was good enough set the node as leaf
//    if (!split_success) {
//      node->SetLeaf(true, data);
//      continue;
//    }
//
//    auto left_child = std::make_shared<Node>(data, node, best_left_split);
//    auto right_child = std::make_shared<Node>(data, node, best_right_split);
//    node->left = left_child;
//    node->right = right_child;
//    node->feature_function = features[best_feature];
//    node->threshold = thresholds[best_feature][best_thr];
//    // Release the vector containing indices in parent node
//    node->samples.clear();
//    node->samples.shrink_to_fit();
//    candidate_nodes.push_back(left_child);
//    candidate_nodes.push_back(right_child);
//  }
//
//  return true;
//}

} // namespace forest
