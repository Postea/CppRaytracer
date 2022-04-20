#pragma once

#include "../tools/EmissionProfile.h"
#include "Material.h"

namespace material {
class Diffuse : public Material {
   public:
	Diffuse(const std::shared_ptr<util::Sampler>& albedo_texture,
	        const std::shared_ptr<util::Sampler>& emission_texture);
	Diffuse(const std::shared_ptr<util::Sampler>& albedo_texture,
	        const std::shared_ptr<util::Sampler>& emission_texture,
	        const util::Image& distribution);
	Diffuse(const std::shared_ptr<util::Sampler>& albedo);
	Diffuse(const util::Vec3& albedo,
	        const util::Vec3& emission = util::Vec3(0));

	util::Vec3 albedo(const std::pair<float, float>& uv) const override;

	util::Vec3 emission(const std::pair<float, float>& uv) const override;

	std::pair<float, float> sample_emission_profile() const override;

	std::optional<util::Vec3> scattered_d(const util::Vec3& d,
	                                      const util::Vec3& n) const override;

	float brdf(const util::Vec3& d_in, const util::Vec3& d_out,
	           const util::Vec3& n) const override;

	float brdf_pdf(const util::Vec3& d_out, const util::Vec3& n) const override;

	std::optional<float> emission_pdf(float u, float v) const override;

   private:
	std::shared_ptr<util::Sampler> albedo_texture;
	util::EmissionProfile emission_profile;
};
}  // namespace material
