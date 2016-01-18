#ifndef RANDOM_FOREST_H_
#define RANDOM_FOREST_H_

#include <vector>
#include <random>

#include "random_tree.h"

namespace forest {

class RandomForest {
 public:
  RandomForest(size_t num_trees, std::vector<TreeParams>& trees_params,
               std::vector<std::shared_ptr<SplitSamplerBase>>& split_samplers);
  //bool Grow();
  bool Grow(const DataBase& data);
  void Test(const DataBase& data);
 private:
  std::vector<RandomTree> trees_;
  // Number of trees to grow
  size_t num_trees_;
  std::vector<TreeParams> trees_params_;
  std::vector<std::shared_ptr<SplitSamplerBase>> split_samplers_;
};

} // namespace forest

#endif  // RANDOM_FOREST_H
