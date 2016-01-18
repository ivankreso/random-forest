#include <iostream>
#include <chrono>

#include "../../random_forest/random_forest.h"
#include "../../random_forest/data_uci.h"
#include "../../random_forest/axis_aligned_split.h"

using namespace forest;

int main(int argc, char** argv) {
  size_t seed;
  if (argc == 2) {
    seed = std::stoi(argv[1]);
  }
  else {
    std::random_device rd;
    seed = rd();
  }

  std::string dataset_path = "/home/kivan/datasets/UCI/IRIS/iris.data";
  std::shared_ptr<DataBase> train_data, test_data;
  double test_split = 0.3;
  DataUCI::CreateDataset(dataset_path, ',', test_split, seed, train_data, test_data);

  TreeParams params;
  // Params for 100% train
  //params.train_percentage = 1.0;
  //params.num_features = 4;
  //params.num_thresholds = 200;
  //params.min_samples_split = 2;
  ////params.min_samples_leaf = 1;
  //params.max_depth = 1000;
  //params.min_gain = 0;

  //params.train_percentage = 0.8;
  params.train_percentage = 1.0;
  //params.num_features = std::round(std::sqrt(train_data->NumFeatures()));
  params.num_features = 2;
  params.num_thresholds = 10;
  //params.min_gain = 0.01;
  params.min_samples_split = 2;
  params.max_depth = 20;
  size_t num_trees = 1000;

  std::vector<TreeParams> trees_params;
  std::vector<std::shared_ptr<SplitSamplerBase>> split_samplers;
  for (size_t i = 0; i < num_trees; i++) {
    trees_params.push_back(params);
    trees_params.back().random_seed = seed + i;
    trees_params.back().rng = std::make_shared<std::mt19937>(seed + i);
    split_samplers.push_back(std::make_shared<AxisAlignedSplitSampler>(*train_data));
  }

  auto start = std::chrono::system_clock::now();
  RandomForest forest(num_trees, trees_params, split_samplers);
  //forest.Grow();
  forest.Grow(*train_data);
  auto end = std::chrono::system_clock::now();
  //auto elapsed = end - start;
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Learning time = " << elapsed.count() << " sec\n";
  std::cout << "Train:" << "\n\t";
  forest.Test(*train_data);
  std::cout << "Test:" << "\n\t";
  forest.Test(*test_data);

  return 0;
}
