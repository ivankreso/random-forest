#ifndef AXIS_ALIGNED_SPLIT_H_
#define AXIS_ALIGNED_SPLIT_H_

#include "split.h"
#include "objective_function.h"
#include "cpp_gems.h"

namespace forest {

class AxisAlignedSplit : public SplitBase {
 public:
  AxisAlignedSplit(size_t dim_num, double threshold);
  SplitType Split(const DataBase& data, size_t sample_num) const override;

 private:
  size_t dim_num_;
  double threshold_;
};

class AxisAlignedSplitSampler : public SplitSamplerBase {
 public:
  AxisAlignedSplitSampler(const DataBase& dataset) : dataset_(dataset) {}

  std::shared_ptr<SplitBase> FindBestSplit(const TreeParams& params,
                                           const std::vector<size_t>& samples,
                                           std::vector<size_t>& left_split,
                                           std::vector<size_t>& right_split) override;
  size_t TrainSize() const override {
    return dataset_.NumSamples();
  }
 private:
  void GenerateSplitCandidates(const TreeParams& params,
                               const std::vector<size_t>& samples,
                               std::vector<size_t>& features,
                               std::vector<std::vector<double>>& thresholds);

  //const TreeParams& params_;
  const DataBase& dataset_;
  std::vector<size_t> features_;
  std::vector<std::vector<double>> thresholds_;
  type::Array2d<std::vector<size_t>> left_splits_;
  type::Array2d<std::vector<size_t>> right_splits_;
};

} // namespace forest

#endif  // AXIS_ALIGNED_SPLIT_H_
