#include "random_forest.h"

#include <iostream>
#include <algorithm>
#include <functional>
#include "forest_helper.h"


namespace forest {

RandomForest::RandomForest(size_t num_trees, std::vector<TreeParams>& trees_params,
                           std::vector<std::shared_ptr<SplitSamplerBase>>& split_samplers) :
    num_trees_(num_trees), split_samplers_(split_samplers) {
  trees_params_ = trees_params;
}

//bool RandomForest::Grow() {
bool RandomForest::Grow(const DataBase& data) {
  //std::random_device rd;
  //size_t seed = rd();
  std::vector<std::vector<size_t>> train_indices;
  for (size_t i = 0; i < num_trees_; i++) {
    //trees_[i] = RandomTree(i, new_params);
    //trees_[i].Grow(data, train_indices);
    trees_.push_back(RandomTree(i, trees_params_[i], split_samplers_[i]));
    //trees_.back().Grow(data, train_indices);
  }
  #pragma omp parallel for
  for (size_t i = 0; i < num_trees_; i++)
    trees_[i].Grow(data);
    //trees_[i].Grow();

  //for (size_t i = 0; i < num_trees_; i++) {
  //  std::vector<std::shared_ptr<Node>> leafs = trees_[i].CollectLeafs();
  //  for (size_t i = 0; i < leafs.size(); i++) {
  //    std::cout << "Leaf " << i << "\nSize = " << leafs[i]->samples.size() << "\n";
  //    if (leafs[i]->entropy > 0) {
  //      std::cout << leafs[i]->entropy << "\n";
  //      throw 1;
  //    }
  //  }
  //}
  return true;
}

void RandomForest::Test(const DataBase& data) {
  if (trees_.size() == 0) {
    std::cout << "[RandomForest]: You must first grow the forest!\n";
    return;
  }

  size_t num_classes = data.NumClasses();
  size_t num_trees = trees_.size();
  std::vector<size_t> predicted_labels;
  for (size_t i = 0; i < data.NumSamples(); i++) {
    std::vector<double> avg_dist(num_classes, 0.0);
    for (size_t j = 0; j < num_trees; j++) {
      auto tree_dist = trees_[j].Predict(data, i);
      //std::transform(avg_dist.begin(), avg_dist.end(), tree_dist.begin(),
      //               std::back_inserter(avg_dist), std::plus<double>());
      std::transform(avg_dist.begin(), avg_dist.end(), tree_dist.begin(),
                     avg_dist.begin(), std::plus<double>());
    }
    //std::cout << avg_dist << "\n";
    std::transform(avg_dist.begin(), avg_dist.end(), avg_dist.begin(),
        [num_trees](double val) { return val / num_trees; });
    //std::cout << avg_dist << "\n";
    double max;
    predicted_labels.push_back(ForestHelper::GetMaxElement(avg_dist, max));
    //std::cout << "Max probability = " << max << "\n";
  }
  size_t correct = 0;
  for (size_t i = 0; i < predicted_labels.size(); i++) {
    //std::cout << predicted_labels[i] << " -- " << data.GetLabel(i) << "\n";
    if (predicted_labels[i] == data.GetLabel(i))
      correct++;
  }
  std::cout << "Accuracy = " << static_cast<double>(correct) / predicted_labels.size() * 100.0
            << "%\n";
}

} // namespace forest
