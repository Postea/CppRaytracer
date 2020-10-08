#pragma once

#include "../tools/Mat4.h"
#include "../tools/Transformation.h"
#include "Shape.h"

namespace shapes {
class SingleGroup : public Shape {
   public:
	SingleGroup(const util::Transformation& trans,
	            std::shared_ptr<Shape> shape);
	SingleGroup(const util::Mat4& matrix, std::shared_ptr<Shape> shape);

	std::optional<cam::Hit> intersect(const cam::Ray& r) const override;
	util::AxisAlignedBoundingBox bounds() const override;

   private:
	util::AxisAlignedBoundingBox boundingVolume;
	std::shared_ptr<Shape> shape;
	util::Transformation transform;
};
}  // namespace shapes
