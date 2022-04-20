#include "Emitting.h"

#include <cassert>

#include "texture/Constant.h"

namespace material {
Emitting::Emitting(const std::shared_ptr<util::Sampler>& texture)
    : emission_profile({texture}) {
}
Emitting::Emitting(const std::shared_ptr<util::Sampler>& texture,
                   const util::Image& distribution, float intensity)
    : emission_profile({texture, distribution, intensity}) {
}
Emitting::Emitting(const util::Vec3& albedo)
    : emission_profile({std::make_shared<Constant>(albedo)}) {
}

// Colour of the light source is only dependend on the emission
util::Vec3 Emitting::albedo(const std::pair<float, float>& uv) const {
	return util::Vec3(1, 1, 1);
}

util::Vec3 Emitting::emission(const std::pair<float, float>& uv) const {
	return emission_profile.color(uv.first, uv.second);
}

std::pair<float, float> Emitting::sample_emission_profile() const {
	return emission_profile.sample();
}

std::optional<float> Emitting::emission_pdf(float u, float v) const {
	return emission_profile.pdf(u, v);
}

std::optional<util::Vec3> Emitting::scattered_d(const util::Vec3& d,
                                                const util::Vec3& n) const {
	return std::nullopt;
}

// Should never get called
float Emitting::brdf(const util::Vec3& d_in, const util::Vec3& d_out,
                     const util::Vec3& n) const {
	// Background should not be able to receive any light
	assert(false);
	return 0;
}
// Should never get called
float Emitting::brdf_pdf(const util::Vec3& d_out, const util::Vec3& n) const {
	// Background can not scatter so this will never be used
	assert(false);
	return 0;
}
}  // namespace material