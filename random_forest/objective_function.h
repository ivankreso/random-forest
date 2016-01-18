#ifndef OBJECTIVE_FUNCTION_H_
#define OBJECTIVE_FUNCTION_H_

#include <iostream>
#include <vector>
#include <cstddef>
#include <cmath>
#include <cassert>

namespace forest {

namespace {
double CalculateEntropy(size_t num_classes, const std::vector<size_t>& labels,
                        const std::vector<size_t>& samples) {
  std::vector<size_t> histogram(num_classes, 0);
  //for (auto cnt : histogram)
  //  std::cout << cnt << " - ";
  for (size_t s : samples)
    histogram[labels[s]]++;
  double entropy = 0.0;
  for (size_t class_cnt : histogram) {
    if (class_cnt > 0) {
      double class_prob = static_cast<double>(class_cnt) / samples.size();
      entropy -= class_prob * std::log2(class_prob);
    }
  }
  return entropy;
}
}

class InformationGain {
 public:
  double Evaluate(size_t num_classes, const std::vector<size_t>& labels,
                  const std::vector<size_t>& parent_samples,
                  const std::vector<size_t>& left_samples,
                  const std::vector<size_t>& right_samples) const {
    assert(parent_samples.size() == left_samples.size() + right_samples.size());
    // Calculate Shannon entropies
    double root_entropy = CalculateEntropy(num_classes, labels, parent_samples);
    double left_entropy = CalculateEntropy(num_classes, labels, left_samples);
    double right_entropy = CalculateEntropy(num_classes, labels, right_samples);
    // Calculate information gain
    double ig = root_entropy - ((left_samples.size() * left_entropy +
                right_samples.size() * right_entropy) / parent_samples.size());
    // Calculate normalized information gain [0, 2.0]
    //std::cout << ig << " -- " << root_entropy << " -- " << left_entropy
    //<< " -- " << right_entropy << "\n";
    double nig = (2.0 * ig) / (root_entropy + left_entropy + right_entropy);
    return nig;
  }
};

//class ObjectiveFunction {
// public:
//  virtual ~ObjectiveFunction() {}
//  // Do we need node_samples
//  virtual double Evaluate(const DataBase& data,
//                          const std::vector<size_t>& node_samples,
//                          const std::vector<size_t>& left_samples,
//                          const std::vector<size_t>& right_samples) const = 0;
//};
//
//class InformationGain : public ObjectiveFunction {
// public:
//  //double Evaluate(const DataBase& data,
//  //                const std::vector<size_t>& left_samples,
//  //                const std::vector<size_t>& right_samples) const override {
//  //  // Calculate Shannon entropies
//  //  double left_entropy = data.CalculateEntropy(left_samples);
//  //  double right_entropy = data.CalculateEntropy(right_samples);
//  //  // Calculate information gain loss
//  //  return (left_samples.size() * left_entropy + right_samples.size() * right_entropy)
//  //          / (left_samples.size() + right_samples.size());
//  //}
//  double Evaluate(const DataBase& data,
//                  const std::vector<size_t>& parent_samples,
//                  const std::vector<size_t>& left_samples,
//                  const std::vector<size_t>& right_samples) const override {
//    assert(parent_samples.size() == left_samples.size() + right_samples.size());
//    // Calculate Shannon entropies
//    double root_entropy = data.CalculateEntropy(parent_samples);
//    double left_entropy = data.CalculateEntropy(left_samples);
//    double right_entropy = data.CalculateEntropy(right_samples);
//    // Calculate information gain
//    double ig = root_entropy - ((left_samples.size() * left_entropy +
//                right_samples.size() * right_entropy) / parent_samples.size());
//    // Calculate normalized information gain [0, 2.0]
//    //std::cout << ig << " -- " << root_entropy << " -- " << left_entropy
//    //<< " -- " << right_entropy << "\n";
//    double nig = (2.0 * ig) / (root_entropy + left_entropy + right_entropy);
//    return nig;
//  }
//};

} // namespace forest

#endif  // FILTER_BASE_H_
