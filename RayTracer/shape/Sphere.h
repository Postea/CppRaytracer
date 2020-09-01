#pragma once

#include "Shape.h"

namespace shapes {
class Sphere : public Shape {
   public:
	Sphere(float radius, const std::shared_ptr<material::Material>& material);
	std::shared_ptr<cam::Hit> intersect(const cam::Ray& r);

   private:
	std::shared_ptr<material::Material> material;
	const float radius;
};
}  // namespace shapes
