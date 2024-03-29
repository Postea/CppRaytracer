#pragma once

#include "Light.h"
#include "Shape.h"

namespace shapes {
class Sphere : public Shape {
   public:
	Sphere(float radius, const std::shared_ptr<material::Material>& material);
	std::optional<cam::Hit> intersect(const cam::Ray& r) const override;
	util::AxisAlignedBoundingBox bounds() const override;

	std::pair<float, float> texture_coordinates(const util::Vec3& pos) const;
	util::Vec3 texture_coordinates(std::pair<float, float> texel) const;

   protected:
	std::shared_ptr<material::Material> material;
	const float radius;
};
}  // namespace shapes
