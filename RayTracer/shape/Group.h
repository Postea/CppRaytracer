#pragma once

#include <vector>

#include "../tools/Mat4.h"
#include "../tools/Transformation.h"
#include "Shape.h"
#include "ShapeSingleGroup.h"

namespace shapes {
class Group : public Shape {
   public:
	Group(const util::Transformation& trans, bool rebuild_bb = true);
	Group(const util::Mat4& matrix, bool rebuild_bb = true);

	std::optional<cam::Hit> intersect(const cam::Ray& r) const override;
	util::AxisAlignedBoundingBox bounds() const override;

	void set_bounds(const util::AxisAlignedBoundingBox& bb);
	void add(const Group& group);
	void add(const ShapeSingleGroup& group);
	void add(const std::shared_ptr<shapes::Shape>& shape);

   protected:
	std::vector<std::shared_ptr<Shape>> shape_list;

   private:
	void rebuild_boundingbox();
	bool rebuild_bb;

	util::AxisAlignedBoundingBox bounding_box;
	util::Transformation transform;
};
}  // namespace shapes
