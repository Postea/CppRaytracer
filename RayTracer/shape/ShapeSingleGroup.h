#pragma once

#include "../tools/Mat4.h"
#include "../tools/Transformation.h"
#include "Shape.h"

namespace shapes {
class ShapeSingleGroup : public Shape {
   public:
	ShapeSingleGroup(const util::Transformation& trans,
	                 std::shared_ptr<Shape> shape);
	ShapeSingleGroup(const util::Mat4& matrix, std::shared_ptr<Shape> shape);

	std::optional<cam::Hit> intersect(const cam::Ray& r) const override;
	std::pair<float, float> texture_coordinates(
	    const util::Vec3& pos) const override;
	util::AxisAlignedBoundingBox bounds() const override;

   private:
	util::AxisAlignedBoundingBox boundingVolume;
	std::shared_ptr<Shape> shape;
	util::Transformation transform;
};
}  // namespace shapes
