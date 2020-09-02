#pragma once

#include "../sampling/Sampler.h"
#include "Material.h"
#include "texture/Constant.h"

namespace material {
class DiffuseMaterial : public Material {
   public:
	DiffuseMaterial(const std::shared_ptr<util::Sampler>& albedo_texture,
	                const std::shared_ptr<util::Sampler>& emission_texture);
	DiffuseMaterial(const std::shared_ptr<util::Sampler>& albedo);
	DiffuseMaterial(const util::Vec3& color);

	util::Vec3 albedo(float texel_x, float texel_y) const override;
	util::Vec3 emission(float texel_x, float texel_y) const override;
	util::Vec3 scattered_d(const util::Vec3& d,
	                       const util::Vec3& n) const override;
	bool scatter() const override;

   private:
	std::shared_ptr<util::Sampler> emission_texture;
	std::shared_ptr<util::Sampler> albedo_texture;
};
}  // namespace material
