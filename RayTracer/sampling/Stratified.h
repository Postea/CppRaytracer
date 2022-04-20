#pragma once

#include "../tools/Random.h"
#include "OptiSampler.h"

namespace util {
class Stratified : public OptiSampler {
   public:
	Stratified(const std::shared_ptr<OptiSampler>& sampler, size_t n);
	std::pair<Vec3, std::vector<int64_t>> color_opti(float x,
	                                                 float y) const override;

   private:
	std::shared_ptr<OptiSampler> sampler;
	size_t n;
};
}  // namespace util