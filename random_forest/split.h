#ifndef SPLIT_H_
#define SPLIT_H_

#include "data_base.h"

namespace forest {

enum SplitType {
  kLeftSplit = 0,
  kRightSplit,
};

class SplitBase {
 public:
  virtual ~SplitBase() {}
  virtual SplitType Split(const DataBase& data, size_t sample_num) const = 0;
};

class SplitSamplerBase {
 public:
  virtual ~SplitSamplerBase() {}
  virtual std::shared_ptr<SplitBase> FindBestSplit(const TreeParams& params,
                                                   const std::vector<size_t>& samples,
                                                   std::vector<size_t>& left_split,
                                                   std::vector<size_t>& right_split) = 0;
  virtual size_t TrainSize() const = 0;
};

} // namespace forest

#endif  // SPLIT_H_
