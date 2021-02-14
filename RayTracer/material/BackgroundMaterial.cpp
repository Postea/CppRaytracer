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
util::Vec3 BackgroundMaterial::albedo(const std::pair<float, float>& uv) const {
	return util::Vec3(1, 1, 1);
}
util::Vec3 BackgroundMaterial::emission(
    const std::pair<float, float>& uv) const {
	return texture->color(uv.first, uv.second);
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