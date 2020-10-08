#pragma once

#include "LightShape.h"

namespace shapes {
class Sphere : public LightShape {
   public:
	Sphere(float radius, const std::shared_ptr<material::Material>& material);
	std::optional<cam::Hit> intersect(const cam::Ray& r) const override;
	util::AxisAlignedBoundingBox bounds() const override;
	util::SurfacePoint sampleLight() const override;

   private:
	std::shared_ptr<material::Material> material;
	const float radius;
};
}  // namespace shapes
