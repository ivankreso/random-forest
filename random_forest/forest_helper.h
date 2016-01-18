#ifndef FOREST_HELPER_H_
#define FOREST_HELPER_H_

#include <iostream>
#include <vector>
#include <set>
#include <random>
#include <cassert>

#include "cpp_gems.h"

namespace forest {
namespace ForestHelper {

// Generate @num uniformly distributed, unique, random numbers with the rng
template <typename T>
std::vector<T> GenerateUniqueIntegers(std::mt19937& rng, size_t num, T min_val, T max_val) {
  assert(max_val > min_val);
  T max_num = max_val - min_val;
  std::uniform_int_distribution<T> distr(min_val, max_val);
  std::set<T> set;
  while (set.size() < num && set.size() <= max_num)
    set.insert(distr(rng));
  return { set.begin(), set.end() };
}

// Generate @num uniformly distributed random numbers with replacements
template <typename T>
std::vector<T> GenerateRandomIntegers(std::mt19937& rng, size_t num, T min_val, T max_val) {
  assert(max_val > min_val);
  std::uniform_int_distribution<T> distr(min_val, max_val);
  std::vector<T> random_ints(num);
  for (size_t i = 0; i < num; i++)
    random_ints[i] = distr(rng);
  return random_ints;
}

template <typename T>
std::vector<T> GenerateRandomReals(std::mt19937& rng, size_t num, T min_val, T max_val) {
  assert(max_val > min_val);
  std::uniform_real_distribution<T> distr(min_val, max_val);
  std::vector<T> vec;
  for (size_t i = 0; i < num; i++)
    vec.push_back(distr(rng));
  return vec;
}

//std::vector<size_t> AggregateHistogram(const DataBase& data, const std::vector<size_t>& samples) {
//  std::vector<size_t> histogram(data.NumClasses(), 0);
//  for (auto s : samples)
//    histogram[data.GetLabel(s)]++;
//  return histogram;
//}
//
//std::vector<double> GetClassDistribution(const DataBase& data, const std::vector<size_t>& samples) {
//  std::vector<size_t> histogram = ForestHelper::AggregateHistogram(data, samples);
//  std::vector<double> class_distribution(data.NumClasses());
//  for (size_t i = 0; i < data.NumClasses(); i++)
//    class_distribution[i] = static_cast<double>(histogram[i]) / samples.size();
//  return class_distribution;
//}

template <typename T>
size_t GetMaxElement(const std::vector<T>& vec, T& max) {
  if (vec.size() == 0) {
    throw 1;
  }
  max = vec[0];
  size_t max_idx = 0;
  for (size_t i = 1; i < vec.size(); i++) {
    if (vec[i] > max) {
      max = vec[i];
      max_idx = i;
    }
  }
  return max_idx;
}

}       // namespace ForestHelper

}       // namespace forest

#endif  // FOREST_HELPER_H_
