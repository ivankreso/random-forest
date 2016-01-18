#ifndef PARAMS_H_
#define PARAMS_H_

#include <random>
#include <memory>
#include "objective_function.h"

namespace forest {

struct TreeParams {
  // The number of features to sample when looking for the best split
  size_t num_features;
  // Number of threshold candidates selected for the evaluation split
  size_t num_thresholds;
  // The maximum depth of the tree
  size_t max_depth;
  // The minimum number of samples required to split an internal node (default=2)
  size_t min_samples_split;
  // The minimum number of samples required to be at a leaf node (default=1)
  //size_t min_samples_leaf;
  // The minimum weighted fraction of the input samples required to be at a leaf node (default=0)
  double min_weight_fraction_leaf;
  // The percentage of train data to randomly sample
  double train_percentage = 0.0;
  // Grow a tree with @max_leaf_nodes in best-first fashion.
  // Best nodes are defined as relative reduction in impurity.
  // Stop if the split loss gain is less then
  double min_loss_decrease;
  double min_gain = 0.0;
  // If 0 then unlimited number of leaf nodes. (default = 0)
  size_t max_leaf_nodes;
  // Random seed to use
  size_t random_seed;
  // Random number generator created with @random_seed
  std::shared_ptr<std::mt19937> rng;
  // Objective function used to evaluate the split (Information Gain for example)
  //std::shared_ptr<ObjectiveFunction> objective_func;
  InformationGain objective_func;
};

} // namespace forest

#endif  // PARAMS_H_
