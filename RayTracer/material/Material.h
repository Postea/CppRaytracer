#pragma once

#include <optional>
#include <random>

#include "../sampling/Sampler.h"
#include "../tools/Vec3.h"

namespace material {

class Material {
   public:
	virtual util::Vec3 albedo(const std::pair<float, float>& uv) const = 0;
	virtual util::Vec3 emission(const std::pair<float, float>& uv) const = 0;
	virtual std::pair<float, float> sampleEmissionProfile() const = 0;
	virtual util::Vec3 scattered_d(const util::Vec3& d,
	                               const util::Vec3& n) const = 0;
	virtual bool scatter(const util::Vec3& d, const util::Vec3& n) const = 0;
	virtual float calculateLightMultiplier(const util::Vec3& d_in,
	                                       const util::Vec3& d_out,
	                                       const util::Vec3& n) const = 0;
	virtual float brdf_pdf(const util::Vec3& d_out,
	                       const util::Vec3& n) const = 0;
	virtual std::optional<float> emission_pdf(float u, float v) const = 0;
};
util::Vec3 reflection(const util::Vec3& d, const util::Vec3& n);
}  // namespace material
