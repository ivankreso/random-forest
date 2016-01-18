#ifndef DATA_BASE_H_
#define DATA_BASE_H_

#include <cstddef>
#include <vector>

#include "params.h"

//namespace forest {
//class DataBase;
//}
//#include "filter_base.h"

namespace forest {

//enum FeatureType {
//  kInteger, kReal, kCategorical
//};
//
//enum ProblemType {
//  CLASSIFICATION, REGRESSION
//};

class DataBase {
 public:
  virtual ~DataBase() {}
  virtual size_t NumSamples() const = 0;
  virtual size_t NumFeatures() const = 0;
  virtual size_t NumClasses() const = 0;
  virtual double GetFeature(size_t sample_num, size_t feature_num) const = 0;
  virtual size_t GetLabel(size_t sample_num) const = 0;

  const std::vector<size_t>& labels() const {
    return labels_;
  }

  std::vector<size_t> AggregateHistogram(const std::vector<size_t>& samples) const {
    std::vector<size_t> histogram(NumClasses(), 0);
    for (auto s : samples)
      histogram[GetLabel(s)]++;
    return histogram;
  }

  std::vector<double> GetClassDistribution() const {
    std::vector<size_t> histogram(NumClasses(), 0);
    for (size_t i = 0; i < NumSamples(); i++)
      histogram[GetLabel(i)]++;
    std::vector<double> class_distribution(NumClasses());
    for (size_t i = 0; i < NumClasses(); i++)
      class_distribution[i] = static_cast<double>(histogram[i]) / NumSamples();
    return class_distribution;
  }

  std::vector<double> GetClassDistribution(const std::vector<size_t>& samples) const {
    std::vector<size_t> histogram = AggregateHistogram(samples);
    std::vector<double> class_distribution(NumClasses());
    for (size_t i = 0; i < NumClasses(); i++)
      class_distribution[i] = static_cast<double>(histogram[i]) / samples.size();
    return class_distribution;
  }

  double CalculateEntropy(const std::vector<size_t>& samples) const {
    std::vector<size_t> histogram(NumClasses(), 0);
    //for (auto cnt : histogram)
    //  std::cout << cnt << " - ";
    for (size_t s : samples) {
      size_t label = GetLabel(s);
      histogram[label]++;
    }
    double entropy = 0.0;
    for (size_t class_cnt : histogram) {
      if (class_cnt > 0) {
        double class_prob = static_cast<double>(class_cnt) / samples.size();
        entropy -= class_prob * std::log2(class_prob);
      }
    }
    return entropy;
  }

 protected:
  std::vector<size_t> labels_;

  //virtual double operator[int n, int dim] = 0;
  //virtual double FeatureResponse(size_t sample_num, const FilterBase& filter) const = 0;
};

} // namespace forest

#endif  // DATA_BASE_H_
