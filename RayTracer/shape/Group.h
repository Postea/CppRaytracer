#pragma once

#include <vector>

#include "../tools/Mat4.h"
#include "../tools/Transformation.h"
#include "Shape.h"
#include "ShapeSingleGroup.h"

namespace shapes {
class Group : public Shape {
   public:
	Group(const util::Transformation& trans);
	Group(const util::Mat4& matrix);

	std::optional<cam::Hit> intersect(const cam::Ray& r) const override;
	util::AxisAlignedBoundingBox bounds() const override;
	void add(const Group& group);
	void add(const ShapeSingleGroup& group);

	// protected:
	void add(const std::shared_ptr<shapes::Shape>& shape);

   private:
	void rebuildBoundingVolume();

	util::AxisAlignedBoundingBox boundingVolume;
	std::vector<std::shared_ptr<Shape>> shapeList;
	util::Transformation transform;
};
}  // namespace shapes
