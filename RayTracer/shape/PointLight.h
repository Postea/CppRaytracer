#pragma once

#include "Light.h"

namespace shapes {
class PointLight : public Light {
   public:
	PointLight(const util::Vec3& emission);

	util::SurfacePoint sampleLight(const cam::Hit& h) const override;
	util::Vec3 calculateLightEmission(const util::SurfacePoint& p,
	                                  const util::Vec3& d) const override;

   private:
	std::shared_ptr<material::Material> material;
};
}  // namespace shapes
