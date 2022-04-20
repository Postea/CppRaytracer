#pragma once

#include "../tools/Random.h"
#include "Sampler.h"

namespace util {
class Stratified : public Sampler {
   public:
	Stratified(const std::shared_ptr<Sampler>& sampler, size_t n);
	Vec3 color(float x, float y) const override;

   private:
	std::shared_ptr<Sampler> sampler;
	size_t n;
};
}  // namespace util
