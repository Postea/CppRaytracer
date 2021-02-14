#pragma once

#include "Material.h"

namespace material {
class BackgroundMaterial : public Material {
   public:
	BackgroundMaterial(const std::shared_ptr<util::Sampler>& texture);
	BackgroundMaterial(const util::Vec3& albedo);

	util::Vec3 albedo(const std::pair<float, float>& uv) const override;
	util::Vec3 emission(const std::pair<float, float>& uv) const override;
	util::Vec3 scattered_d(const util::Vec3& d,
	                       const util::Vec3& n) const override;
	bool scatter(const util::Vec3& d, const util::Vec3& n) const override;

	float calculateLightMultiplier(const util::Vec3& d_in,
	                               const util::Vec3& d_out,
	                               const util::Vec3& n) const override;

   private:
	std::shared_ptr<util::Sampler> texture;
};
}  // namespace material
