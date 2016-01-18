#ifndef RANDOM_TREE_H_
#define RANDOM_TREE_H_

#include <vector>
#include <memory>
#include <random>

#include "params.h"
#include "split.h"
#include "objective_function.h"
#include "forest_helper.h"

namespace forest {

struct Node {
  Node(const DataBase& data, const std::vector<size_t>& sample_indices) {
    samples = sample_indices;
    depth = 0;
    entropy = data.CalculateEntropy(samples);
  }
  Node(const DataBase& data, const std::shared_ptr<Node>& parent_node,
       std::vector<size_t>& sample_indices) : parent(parent_node) {
    samples = std::move(sample_indices);
    depth = parent_node->depth + 1;
    entropy = data.CalculateEntropy(samples);
  }
  inline
  //void DeclareLeaf(const std::vector<size_t>& labels) {
  void DeclareLeaf(const DataBase& data) {
    is_leaf = true;
    //UpdateClassDistribution(labels);
    UpdateClassDistribution(data);
  }
  inline
  //void UpdateClassDistribution(const DataBase& data, const std::vector<size_t>& labels) {
  void UpdateClassDistribution(const DataBase& data) {
    class_distribution = data.GetClassDistribution(samples);
  }
  SplitType Split(const DataBase& data, size_t sample) {
    return splitter_->Split(data, sample);
  }
  //bool is_leaf() const {
  //  return is_leaf_;
  //}
  //std::vector<size_t>& samples() {
  //  return samples_;
  //}

 //private:
  std::vector<size_t> samples;
  std::weak_ptr<Node> parent;
  std::shared_ptr<Node> left = nullptr;
  std::shared_ptr<Node> right = nullptr;
  // This function contains the feature filter
  std::shared_ptr<SplitBase> splitter_;
  // This value stores the threshold used for splitting
  double entropy;
  // Depth of the node
  size_t depth;
  bool is_leaf = false;
  // Class distribution (we use them only in leaf nodes)
  std::vector<double> class_distribution;
  // This function contains the splitting criteria
  //std::shared_ptr<SplitterBase> splitter_;
};

class RandomTree {
 public:
  //RandomTree(size_t tree_id, TreeParams& params, std::shared_ptr<SplitSamplerBase> split_sampler);
  RandomTree(size_t tree_id, TreeParams& params, std::shared_ptr<SplitSamplerBase>& split_sampler);
  //bool Grow();
  bool Grow(const DataBase& data);
  bool StopSplitting(const Node& node);
  std::vector<double> Predict(const DataBase& data, size_t sample_num) const;
  std::vector<std::shared_ptr<Node>> CollectLeafs() const {
    return CollectSubtreeLeafs(root_);
  }
  std::vector<size_t> SampleTrainingSubset();

  int tree_id() const {
    return tree_id_;
  }

 private:
  //void UpdateClassDistributions(const DataBase& data, const std::vector<size_t>& train_subset);
  std::vector<std::shared_ptr<Node>> CollectSubtreeLeafs(
      const std::shared_ptr<Node> subtree) const;

  std::shared_ptr<Node> root_;
  int tree_id_;
  TreeParams params_;
  //std::shared_ptr<SplitSamplerBase> split_sampler_;
  std::shared_ptr<SplitSamplerBase>& split_sampler_;
  //std::vector<double> class_distribution_full_;
  //std::vector<double> class_distribution_subset_;
  std::vector<size_t> train_subset_;
};

} // namespace forest

#endif  // RANDOM_FOREST_H
