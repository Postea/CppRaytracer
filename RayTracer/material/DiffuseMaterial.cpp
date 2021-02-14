#define _USE_MATH_DEFINES
#include "DiffuseMaterial.h"

#include <math.h>

#include <cassert>

#include "../tools/Random.h"
#include "texture/Constant.h"

namespace material {
DiffuseMaterial::DiffuseMaterial(
    const std::shared_ptr<util::Sampler>& albedo_texture,
    const std::shared_ptr<util::Sampler>& emission_texture)
    : albedo_texture(albedo_texture), emission_texture(emission_texture) {
}
DiffuseMaterial::DiffuseMaterial(
    const std::shared_ptr<util::Sampler>& albedo_texture)
    : albedo_texture(albedo_texture),
      emission_texture(
          std::make_shared<Constant>(Constant(util::Vec3(0, 0, 0)))) {
}
DiffuseMaterial::DiffuseMaterial(const util::Vec3& color)
    : albedo_texture(std::make_shared<Constant>(Constant(color))),
      emission_texture(
          std::make_shared<Constant>(Constant(util::Vec3(0, 0, 0)))) {
}
util::Vec3 DiffuseMaterial::albedo(const std::pair<float, float>& uv) const {
	return albedo_texture->color(uv.first, uv.second);
}
util::Vec3 DiffuseMaterial::emission(const std::pair<float, float>& uv) const {
	return emission_texture->color(uv.first, uv.second);
}
util::Vec3 DiffuseMaterial::scattered_d(const util::Vec3& d,
                                        const util::Vec3& n) const {
	util::Vec3 rand = util::rand_vec3_in_circle(1);
	util::Vec3 result = n + rand;

	return result;
}
bool DiffuseMaterial::scatter(const util::Vec3& d, const util::Vec3& n) const {
	return true;
}
float DiffuseMaterial::calculateLightMultiplier(const util::Vec3& d_in,
                                                const util::Vec3& d_out,
                                                const util::Vec3& n) const {
	return M_1_PI;
}
}  // namespace material