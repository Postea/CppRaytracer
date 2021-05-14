#pragma once

#include "Light.h"

namespace shapes {
class PointLight : public Light {
   public:
	PointLight(const util::Vec3& emission);

	util::SurfacePoint sampleLight(const cam::Hit& h) const override;
	util::Vec3 lightEmission(const util::SurfacePoint& p) const override;
	float lightPdf(const util::SurfacePoint& p,
	               const util::Vec3& dl_out) const override;

   private:
	std::shared_ptr<material::Material> material;
};
}  // namespace shapes
