#define _USE_MATH_DEFINES
#include "Diffuse.h"

#include <math.h>

#include <cassert>

#include "../tools/Random.h"
#include "texture/Constant.h"

namespace material {
Diffuse::Diffuse(const std::shared_ptr<util::Sampler>& albedo_texture,
                 const std::shared_ptr<util::Sampler>& emission_texture)
    : albedo_texture(albedo_texture), emission_profile({emission_texture}) {
}
Diffuse::Diffuse(const std::shared_ptr<util::Sampler>& albedo_texture,
                 const std::shared_ptr<util::Sampler>& emission_texture,
                 const util::Image& distribution)
    : albedo_texture(albedo_texture),
      emission_profile({emission_texture, distribution}) {
}
Diffuse::Diffuse(const std::shared_ptr<util::Sampler>& albedo_texture)
    : albedo_texture(albedo_texture),
      emission_profile(
          {std::make_shared<Constant>(Constant(util::Vec3(0, 0, 0)))}) {
}
Diffuse::Diffuse(const util::Vec3& albedo, const util::Vec3& emission)
    : albedo_texture(std::make_shared<Constant>(albedo)),
      emission_profile(std::make_shared<Constant>(emission)) {
}
// Depends on the albedo texture
util::Vec3 Diffuse::albedo(const std::pair<float, float>& uv) const {
	return albedo_texture->color(uv.first, uv.second);
}
// Depends on the emission profile
util::Vec3 Diffuse::emission(const std::pair<float, float>& uv) const {
	return emission_profile.color(uv.first, uv.second);
}
std::pair<float, float> Diffuse::sample_emission_profile() const {
	return emission_profile.sample();
}

std::optional<float> Diffuse::emission_pdf(float u, float v) const {
	return emission_profile.pdf(u, v);
}
std::optional<util::Vec3> Diffuse::scattered_d(const util::Vec3& d,
                                               const util::Vec3& n) const {
	util::Vec3 rand = util::rand_vec3_in_circle(1);
	util::Vec3 result = n + rand;

	return std::make_optional(result);
}
float Diffuse::brdf(const util::Vec3& d_in, const util::Vec3& d_out,
                    const util::Vec3& n) const {
	return M_1_PI;
}

float Diffuse::brdf_pdf(const util::Vec3& d_out, const util::Vec3& n) const {
	auto dot = util::dot(n, d_out.normalize());
	dot = std::max<float>(dot, 0);
	return dot * M_1_PI;
}
}  // namespace material