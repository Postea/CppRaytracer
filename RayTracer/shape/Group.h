#pragma once

#include <vector>

#include "../tools/Mat4.h"
#include "../tools/Transformation.h"
#include "Shape.h"

namespace shapes {
class Group : public Shape {
   public:
	Group(const util::Transformation& trans);
	Group(const util::Mat4& matrix);

	std::shared_ptr<cam::Hit> intersect(const cam::Ray& r) const override;
	util::AxisAlignedBoundingBox bounds() const override;
	// protected:TODO
	void add(const std::shared_ptr<shapes::Shape>& shape);

   private:
	void rebuildBoundingVolume();

	util::AxisAlignedBoundingBox boundingVolume;
	std::vector<std::shared_ptr<Shape>> shapeList;
	util::Transformation transform;
};
}  // namespace shapes
