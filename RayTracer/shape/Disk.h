#pragma once

#include "Shape.h"

namespace shapes {
class Disk : public Shape {
   public:
	Disk(float radius, bool twofaced,
	     const std::shared_ptr<material::Material>& material);

	std::optional<cam::Hit> intersect(const cam::Ray& r) const override;
	util::AxisAlignedBoundingBox bounds() const override;

	std::pair<float, float> texture_coordinates(const util::Vec3& pos) const;

   private:
	std::shared_ptr<material::Material> material;
	const float radius;
	const bool twofaced;
};
}  // namespace shapes
