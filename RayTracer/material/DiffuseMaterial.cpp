#include "DiffuseMaterial.h"

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
util::Vec3 DiffuseMaterial::albedo(float texel_x, float texel_y) const {
	return albedo_texture->color(texel_x, texel_y);
}
util::Vec3 DiffuseMaterial::emission(float texel_x, float texel_y) const {
	return emission_texture->color(texel_x, texel_y);
}
util::Vec3 DiffuseMaterial::scattered_d(const util::Vec3& d,
                                        const util::Vec3& n) const {
	util::Vec3 rand = util::rand_vec3_in_circle(1);
	util::Vec3 result = n + rand;
	/*if (util::dot(n, result) <= 0) {
	    std::cout << n << " " << rand << std::endl;
	    assert(false);
	}*/
	return result;
}
bool DiffuseMaterial::scatter(const util::Vec3& d, const util::Vec3& n) const {
	return true;
}
float DiffuseMaterial::calculateLightMultiplier(const util::Vec3& d_in,
                                                const util::Vec3& d_out,
                                                const util::Vec3& n) const {
	return util::dot(n, d_in);
}
}  // namespace material