#include "BackgroundMaterial.h"

#include <cassert>

#include "texture/Constant.h"

namespace material {
BackgroundMaterial::BackgroundMaterial(
    const std::shared_ptr<util::Sampler>& texture)
    : emission_profile({texture}) {
}
BackgroundMaterial::BackgroundMaterial(
    const std::shared_ptr<util::Sampler>& texture,
    const util::Image& distribution, float intensity)
    : emission_profile({texture, distribution, intensity}) {
}
BackgroundMaterial::BackgroundMaterial(const util::Vec3& albedo)
    : emission_profile({std::make_shared<Constant>(albedo)}) {
}
// Everything in the background material is derived from its emission profile
util::Vec3 BackgroundMaterial::albedo(const std::pair<float, float>& uv) const {
	return util::Vec3(1, 1, 1);
}
util::Vec3 BackgroundMaterial::emission(
    const std::pair<float, float>& uv) const {
	return emission_profile.color(uv.first, uv.second);
}
// This gives us the sampled points uv, which is than used to get the value of
// the pdf and the emission value of the texture
std::pair<float, float> BackgroundMaterial::sampleEmissionProfile() const {
	return emission_profile.sample();
}

std::optional<float> BackgroundMaterial::emission_pdf(float u, float v) const {
	return emission_profile.pdf(u, v);
}
// Gets called because scatter checks for the outgoing vector.
// Only really used for reflective material
std::optional<util::Vec3> BackgroundMaterial::scattered_d(
    const util::Vec3& d, const util::Vec3& n) const {
	return std::nullopt;
}
/*
bool BackgroundMaterial::scatter(const util::Vec3& d,
                                 const util::Vec3& n) const {
    return false;
}*/

float BackgroundMaterial::calculateLightMultiplier(const util::Vec3& d_in,
                                                   const util::Vec3& d_out,
                                                   const util::Vec3& n) const {
	// Background should not be able to receive any light
	assert(false);
	return 0;
}
// Should never get called
float BackgroundMaterial::brdf_pdf(const util::Vec3& d_out,
                                   const util::Vec3& n) const {
	// Background can not scatter so this is fine and will never be used
	assert(false);
	return 0;
}
}  // namespace material