#pragma once

#include "Shape.h"

namespace shapes {
class Background : public Shape {
   public:
	Background(const std::shared_ptr<material::Material>& material);
	std::shared_ptr<cam::Hit> intersect(const cam::Ray& r);

   private:
	std::shared_ptr<material::Material> material;
};
}  // namespace shapes
