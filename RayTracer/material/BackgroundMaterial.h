#pragma once

#include "Material.h"

namespace material {
class BackgroundMaterial : public Material {
   public:
	BackgroundMaterial(const std::shared_ptr<util::Sampler>& texture);
	BackgroundMaterial(const util::Vec3& albedo);

	util::Vec3 albedo(float texel_x, float texel_y) const override;
	util::Vec3 emission(float texel_x, float texel_y) const override;
	util::Vec3 scattered_d(const util::Vec3& d,
	                       const util::Vec3& n) const override;
	bool scatter() const override;

   private:
	std::shared_ptr<util::Sampler> texture;
};
}  // namespace material
