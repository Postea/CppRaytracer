#pragma once

#include <vector>

#include "../tools/Mat4.h"
#include "../tools/Transformation.h"
#include "Shape.h"
#include "ShapeSingleGroup.h"

namespace shapes {
class Group : public Shape {
   public:
	Group(const util::Transformation& trans, bool rebuildBB = true);
	Group(const util::Mat4& matrix, bool rebuildBB = true);

	std::optional<cam::Hit> intersect(const cam::Ray& r) const override;
	util::AxisAlignedBoundingBox bounds() const override;
	void setBounds(const util::AxisAlignedBoundingBox& bb);
	void add(const Group& group);
	void add(const ShapeSingleGroup& group);

	// protected:
	void add(const std::shared_ptr<shapes::Shape>& shape);

	std::vector<std::shared_ptr<Shape>> shapeList;

   private:
	void rebuildBoundingVolume();
	bool rebuildBB;

	util::AxisAlignedBoundingBox boundingVolume;
	util::Transformation transform;
};
}  // namespace shapes
