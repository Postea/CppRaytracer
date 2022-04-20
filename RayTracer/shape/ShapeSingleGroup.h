#pragma once

#include "../tools/Mat4.h"
#include "../tools/Transformation.h"
#include "Shape.h"

namespace shapes {
class ShapeSingleGroup : public Shape {
   public:
	ShapeSingleGroup(const util::Transformation& trans,
	                 const std::shared_ptr<Shape>& shape);
	ShapeSingleGroup(const util::Mat4& matrix,
	                 const std::shared_ptr<Shape>& shape);

	std::optional<cam::Hit> intersect(const cam::Ray& r) const override;
	util::AxisAlignedBoundingBox bounds() const override;

   private:
	util::AxisAlignedBoundingBox bounding_box;
	std::shared_ptr<Shape> shape;
	util::Transformation transform;
};
}  // namespace shapes
