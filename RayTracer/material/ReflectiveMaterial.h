#pragma once

#include "../tools/EmissionProfile.h"
#include "Material.h"

namespace material {
class ReflectiveMaterial : public Material {
   public:
	ReflectiveMaterial(const std::shared_ptr<util::Sampler>& albedo_texture,
	                   const std::shared_ptr<util::Sampler>& emission_texture,
	                   float clearness);
	ReflectiveMaterial(const std::shared_ptr<util::Sampler>& albedo,
	                   float clearness);
	ReflectiveMaterial(const util::Vec3& color, float clearness);

	util::Vec3 albedo(const std::pair<float, float>& uv) const override;
	util::Vec3 emission(const std::pair<float, float>& uv) const override;
	util::Vec3 scattered_d(const util::Vec3& d,
	                       const util::Vec3& n) const override;
	bool scatter(const util::Vec3& d, const util::Vec3& n) const override;

	float calculateLightMultiplier(const util::Vec3& d_in,
	                               const util::Vec3& d_out,
	                               const util::Vec3& n) const override;
	float brdf_pdf(const util::Vec3& d_out, const util::Vec3& n) const override;

   private:
	std::shared_ptr<util::Sampler> albedo_texture;
	util::EmissionProfile emission_profile;
	float clearness;
};
}  // namespace material
