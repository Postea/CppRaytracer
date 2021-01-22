#define _USE_MATH_DEFINES
#include "ReflectiveMaterial.h"

#include <cassert>

#include "../tools/Random.h"
#include "texture/Constant.h"

namespace material {
ReflectiveMaterial::ReflectiveMaterial(
    const std::shared_ptr<util::Sampler>& albedo_texture,
    const std::shared_ptr<util::Sampler>& emission_texture, float clearness)
    : albedo_texture(albedo_texture),
      emission_texture(emission_texture),
      clearness(clearness) {
}
ReflectiveMaterial::ReflectiveMaterial(
    const std::shared_ptr<util::Sampler>& albedo_texture, float clearness)
    : albedo_texture(albedo_texture),
      emission_texture(
          std::make_shared<Constant>(Constant(util::Vec3(0, 0, 0)))),
      clearness(clearness) {
}
ReflectiveMaterial::ReflectiveMaterial(const util::Vec3& color, float clearness)
    : albedo_texture(std::make_shared<Constant>(Constant(color))),
      emission_texture(
          std::make_shared<Constant>(Constant(util::Vec3(0, 0, 0)))),
      clearness(clearness) {
}
util::Vec3 ReflectiveMaterial::albedo(float texel_x, float texel_y) const {
	return albedo_texture->color(texel_x, texel_y);
}
util::Vec3 ReflectiveMaterial::emission(float texel_x, float texel_y) const {
	return emission_texture->color(texel_x, texel_y);
}
util::Vec3 ReflectiveMaterial::scattered_d(const util::Vec3& d,
                                           const util::Vec3& n) const {
	util::Vec3 rand = util::rand_vec3_in_circle(clearness);
	util::Vec3 reflected = reflection(d, n);
	return reflected + rand;
}
bool ReflectiveMaterial::scatter(const util::Vec3& d,
                                 const util::Vec3& n) const {
	return util::dot(d, n) > 0;
}
float ReflectiveMaterial::calculateLightMultiplier(const util::Vec3& d_in,
                                                   const util::Vec3& d_out,
                                                   const util::Vec3& n) const {
	util::Vec3 reflected = reflection(-d_out, n).normalize();
	/*if (std::pow(util::dot(reflected, -d_in), 1) < 0.7) {
	    std::cout << reflected << " " << -d_in << " "
	              << util::dot(reflected, -d_in) << std::endl;
	    assert(false);
	}*/
	auto dot = util::dot(reflected, -d_in);
	auto theta = std::acos(dot);

	return std::cos(std::min<float>(theta * 3, M_PI / 2));
}
}  // namespace material