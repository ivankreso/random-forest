#include "axis_aligned_split.h"
#include "forest_helper.h"

namespace forest {

AxisAlignedSplit::AxisAlignedSplit(size_t dim_num, double threshold) :
    dim_num_(dim_num), threshold_(threshold) {
}

SplitType AxisAlignedSplit::Split(const DataBase& data, size_t sample_num) const {
  if (data.GetFeature(sample_num, dim_num_) < threshold_)
    return SplitType::kLeftSplit;
  return SplitType::kRightSplit;
}

// AxisAlignedSplitSampler

void AxisAlignedSplitSampler::GenerateSplitCandidates(const TreeParams& params,
                                                      const std::vector<size_t>& samples,
                                                      std::vector<size_t>& features,
                                                      std::vector<std::vector<double>>& thresholds) {
  features = ForestHelper::GenerateUniqueIntegers<size_t>(*params.rng, params.num_features, 0,
                                                          dataset_.NumFeatures() - 1);
  thresholds.clear();
  thresholds.resize(features.size());
  for (size_t i = 0; i < features.size(); i++) {
    std::vector<size_t> random_samples_idx = ForestHelper::
        GenerateUniqueIntegers<size_t>(*params.rng, params.num_thresholds, 0, samples.size()-1);
    for (auto idx : random_samples_idx)
      thresholds[i].push_back(dataset_.GetFeature(samples[idx], features[i]));
    //std::vector<double> thrs;
    //FindFeatureMinMax(samples, feature, min, max);
    //thrs = ForestHelper::GenerateRandomReals<double>(*params.rng, params.num_thresholds, min, max);
    //thresholds.push_back(std::move(thrs));
  }
}

std::shared_ptr<SplitBase> AxisAlignedSplitSampler::FindBestSplit(
    const TreeParams& params,
    const std::vector<size_t>& samples,
    std::vector<size_t>& left_split,
    std::vector<size_t>& right_split) {
  GenerateSplitCandidates(params, samples, features_, thresholds_);

  left_splits_.clear();
  left_splits_.resize(features_.size(), thresholds_.size());
  right_splits_.clear();
  right_splits_.resize(features_.size(), thresholds_.size());

  for (size_t i = 0; i < samples.size(); i++) {
    for (size_t j = 0; j < features_.size(); j++) {
      for (size_t k = 0; k < thresholds_.size(); k++) {
        if (dataset_.GetFeature(samples[i], features_[j]) < thresholds_[j][k])
          left_splits_[j][k].push_back(samples[i]);
        else
          right_splits_[j][k].push_back(samples[i]);
      }
    }
  }

  size_t best_feature, best_thr;
  bool split_success = false;
  double best_gain = std::numeric_limits<double>::min();
  for (size_t i = 0; i < features_.size(); i++) {
    for (size_t j = 0; j < thresholds_.size(); j++) {
      double gain = params.objective_func.Evaluate(dataset_.NumClasses(), dataset_.labels(), samples,
                                                   left_splits_[i][j], right_splits_[i][j]);
      assert(gain >= -1e-10 && gain <= 2.0);
      //std::cout << gain << "\n";
      // If the split is the best so far, save it
      //if (loss < best_loss) {
      if (gain > best_gain && gain >= params.min_gain) {
        split_success = true;
        //best_loss = loss;
        best_gain = gain;
        best_feature = i;
        best_thr = j;
      }
    }
  }

  std::shared_ptr<SplitBase> split;
  if (split_success) {
    split = std::make_shared<AxisAlignedSplit>(features_[best_feature],
                                               thresholds_[best_feature][best_thr]);
    left_split = std::move(left_splits_[best_feature][best_thr]);
    right_split = std::move(right_splits_[best_feature][best_thr]);
  }
  return split;
}

}
