#include "BackgroundMaterial.h"

#include "texture/Constant.h"

namespace material {
BackgroundMaterial::BackgroundMaterial(
    const std::shared_ptr<util::Sampler>& texture)
    : texture(texture) {
}
BackgroundMaterial::BackgroundMaterial(const util::Vec3& albedo)
    : texture(std::make_shared<Constant>(albedo)) {
}
util::Vec3 BackgroundMaterial::albedo(float texel_x, float texel_y) const {
	return util::Vec3(1, 1, 1);
}
util::Vec3 BackgroundMaterial::emission(float texel_x, float texel_y) const {
	return texture->color(texel_x, -texel_y);
}
util::Vec3 BackgroundMaterial::scattered_d(const util::Vec3& d,
                                           const util::Vec3& n) const {
	return util::Vec3(0, 0, 0);
}
bool BackgroundMaterial::scatter(const util::Vec3& d,
                                 const util::Vec3& n) const {
	return false;
}

float BackgroundMaterial::calculateLightMultiplier(const util::Vec3& d_in,
                                                   const util::Vec3& d_out,
                                                   const util::Vec3& n) const {
	// Background should not be able to receive any light
	return 0;
}
}  // namespace material