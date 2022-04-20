#pragma once

#include <optional>

#include "../sampling/Image.h"
#include "../sampling/Sampler.h"

namespace util {
class EmissionProfile : public Sampler {
   public:
	// Uniform distribution constructor
	EmissionProfile(const std::shared_ptr<util::Sampler>& emission,
	                float intensity = 1);
	// Image distribution constructor
	EmissionProfile(const std::shared_ptr<util::Sampler>& emission,
	                const Image& distribution, float intensity = 1);

	// Returns a texels based on distribution
	std::pair<float, float> sample() const;
	util::Vec3 color(float x, float y) const override;
	std::optional<float> pdf(float x, float y) const;

   private:
	std::pair<std::pair<float, float>, float> binary_search(float target,
	                                                        int start = 0,
	                                                        int end = -1) const;
	std::pair<std::pair<float, float>, float> exponential_search(
	    float target) const;

	std::shared_ptr<util::Sampler> emission;
	std::optional<Image> distribution;
	std::vector<std::pair<std::pair<float, float>, float>> cdf;
	bool uniform;
	float intensity;
	float normalizer;
};

}  // namespace util