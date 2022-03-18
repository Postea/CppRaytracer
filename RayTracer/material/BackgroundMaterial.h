#pragma once

#include "../tools/EmissionProfile.h"
#include "Material.h"

namespace material {
class BackgroundMaterial : public Material {
   public:
	BackgroundMaterial(const std::shared_ptr<util::Sampler>& texture);
	BackgroundMaterial(const std::shared_ptr<util::Sampler>& texture,
	                   const util::Image& distribution, float intensity = 1);
	BackgroundMaterial(const util::Vec3& albedo);

	util::Vec3 albedo(const std::pair<float, float>& uv) const override;
	util::Vec3 emission(const std::pair<float, float>& uv) const override;
	std::pair<float, float> sampleEmissionProfile() const override;
	std::optional<util::Vec3> scattered_d(const util::Vec3& d,
	                                      const util::Vec3& n) const override;
	// bool scatter(const util::Vec3& d, const util::Vec3& n) const override;
	float calculateLightMultiplier(const util::Vec3& d_in,
	                               const util::Vec3& d_out,
	                               const util::Vec3& n) const override;
	float brdf_pdf(const util::Vec3& d_out, const util::Vec3& n) const override;
	std::optional<float> emission_pdf(float u, float v) const override;

   private:
	util::EmissionProfile emission_profile;
};
}  // namespace material
