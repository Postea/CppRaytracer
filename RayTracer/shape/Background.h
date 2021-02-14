#pragma once

#include "Shape.h"

namespace shapes {
class Background : public Shape {
   public:
	Background(const std::shared_ptr<material::Material>& material);
	std::optional<cam::Hit> intersect(const cam::Ray& r) const override;
	std::pair<float, float> Background::texture_coordinates(
	    const util::Vec3& pos) const override;
	util::AxisAlignedBoundingBox bounds() const override;

   private:
	std::shared_ptr<material::Material> material;
};
}  // namespace shapes
