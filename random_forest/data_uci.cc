#include "data_uci.h"
#include "forest_helper.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <random>

namespace {

std::vector<std::string> Split(const std::string& line, char delim) {
  std::vector<std::string> elems;
  std::stringstream ss(line);
  std::string item;
  while (std::getline(ss, item, delim))
    elems.push_back(item);
  return elems;
}

} // empty namespace

namespace forest {

bool DataUCI::CreateDataset(std::string file_path, char delimiter, double test_ratio, size_t seed,
                            std::shared_ptr<DataBase>& train, std::shared_ptr<DataBase>& test) {
  //std::random_device rd;
  //std::mt19937 gen(rd());
  std::mt19937 gen(seed);
  std::uniform_real_distribution<> udist(0, 1);

  auto train_ptr = std::make_shared<DataUCI>();
  auto test_ptr = std::make_shared<DataUCI>();
  std::ifstream freader(file_path);
  std::string line;
  bool is_train;
  //DataUCI& train_ref = dynamic_cast<DataUCI&>(*train);
  //DataUCI& test_ref = dynamic_cast<DataUCI&>(*test);
  auto train_data = train_ptr->data();
  auto train_labels = train_ptr->labels();
  auto test_data = test_ptr->data();
  auto test_labels = test_ptr->labels();
  std::unordered_map<std::string,size_t> class_map;  
  size_t num_classes = 0;
  while (std::getline(freader, line)) {
    if (line.size() == 0)
      break;
    std::vector<std::string> split = Split(line, delimiter);
    std::vector<double> features;
    for (size_t i = 0; i < split.size()-1; i++) {
      features.push_back(std::stod(split[i]));
      //std::cout << features[i] << "\t";
    }
    if (class_map.find(split.back()) == class_map.end())
      class_map[split.back()] = num_classes++;
    size_t class_id = class_map[split.back()];
    //std::cout << "-> " << class_id << "\n";

    // determine weather to put sample in train or test
    if (udist(gen) > test_ratio)
      is_train = true;
    else
      is_train = false;
    if (is_train) {
      train_data.push_back(features);
      train_labels.push_back(class_id);
    }
    else {
      test_data.push_back(features);
      test_labels.push_back(class_id);
    }
  }
  train_ptr->class_map() = class_map;
  test_ptr->class_map() = class_map;
  train_ptr->data() = train_data;
  test_ptr->data() = test_data;
  train_ptr->labels() = train_labels;
  test_ptr->labels() = test_labels;
  train = std::dynamic_pointer_cast<DataBase>(train_ptr);
  test = std::dynamic_pointer_cast<DataBase>(test_ptr);

  return true;
}

void DataUCI::FindFeatureMinMax(const std::vector<size_t>& samples, size_t feature,
                                double& min, double& max) const {
  assert(samples.size() > 1);
  max = data_[samples[0]][feature];
  min = data_[samples[0]][feature];
  for (size_t i = 1; i < samples.size(); i++) {
    double val = data_[samples[i]][feature];
    if (val > max) max = val;
    else if (val < min) min = val;
  }
}

//void DataUCI::UpdateFeaturesMinMax() {
//  for (size_t i = 0; i < NumFeatures(); i++)
//    FindFeatureMinMax(i, feature_min_[i], feature_max_[i]);
//}

} // namespace forest

