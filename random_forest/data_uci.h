#ifndef DATA_UCI_H_
#define DATA_UCI_H_

#include <string>
#include <memory>
#include <unordered_map>
#include <cassert>

#include "data_base.h"

namespace forest {

class DataUCI : public DataBase {
 public:
  size_t NumSamples() const override {
    return data_.size();
  }
  size_t NumFeatures() const override {
    return data_[0].size();
  }
  size_t NumClasses() const override {
    return class_map_.size();
  }
  double GetFeature(size_t sample_num, size_t feature_num) const override {
    assert(sample_num < data_.size() && feature_num < NumFeatures());
    return data_[sample_num][feature_num];
  }
  size_t GetLabel(size_t sample_num) const override {
    assert(sample_num < NumSamples());
    return labels_[sample_num];
  }
  void FindFeatureMinMax(const std::vector<size_t>& samples, size_t feature,
                         double& min, double& max) const;

  static bool CreateDataset(std::string file_path, char delimiter, double test_ratio, size_t seed,
                            std::shared_ptr<DataBase>& train, std::shared_ptr<DataBase>& test);

  std::vector<std::vector<double>>& data() {
    return data_;
  }
  std::vector<size_t>& labels() {
    return labels_;
  }
  std::unordered_map<std::string,size_t>& class_map() {
    return class_map_;
  }
 private:
  std::vector<std::vector<double>> data_;
  std::unordered_map<std::string,size_t> class_map_;
  std::vector<double> class_distribution_;
};

} // namespace forest

#endif  // DATA_UCI_H_
