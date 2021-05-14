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
      emission_profile({emission_texture}),
      clearness(clearness) {
}
ReflectiveMaterial::ReflectiveMaterial(
    const std::shared_ptr<util::Sampler>& albedo_texture, float clearness)
    : albedo_texture(albedo_texture),
      emission_profile(
          {std::make_shared<Constant>(Constant(util::Vec3(0, 0, 0)))}),
      clearness(clearness) {
}
ReflectiveMaterial::ReflectiveMaterial(const util::Vec3& color, float clearness)
    : albedo_texture(std::make_shared<Constant>(Constant(color))),
      emission_profile(
          {std::make_shared<Constant>(Constant(util::Vec3(0, 0, 0)))}),
      clearness(clearness) {
}
util::Vec3 ReflectiveMaterial::albedo(const std::pair<float, float>& uv) const {
	return albedo_texture->color(uv.first, uv.second);
}
util::Vec3 ReflectiveMaterial::emission(
    const std::pair<float, float>& uv) const {
	return emission_profile.color(uv.first, uv.second);
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

float ReflectiveMaterial::brdf_pdf(const util::Vec3& d_out,
                                   const util::Vec3& n) const {
	// This is gonna be a problem, because this material can scatter
	return 0;
}
}  // namespace material