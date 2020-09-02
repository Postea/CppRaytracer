#include "DiffuseMaterial.h"

#include "../tools/Vec3.h"
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
	return n + rand_vec3_in_circle(1);
}
bool DiffuseMaterial::scatter() const {
	return true;
}
}  // namespace material