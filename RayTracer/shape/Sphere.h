#pragma once

#include "Light.h"
#include "Shape.h"

namespace shapes {
class Sphere : public Shape, public Light {
   public:
	Sphere(float radius, const std::shared_ptr<material::Material>& material);
	std::optional<cam::Hit> intersect(const cam::Ray& r) const override;
	util::AxisAlignedBoundingBox bounds() const override;
	cam::Hit sampleLight() const override;

   private:
	std::shared_ptr<material::Material> material;
	const float radius;
};
}  // namespace shapes
