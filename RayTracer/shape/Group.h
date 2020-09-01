#pragma once

#include <vector>

#include "../camera/Hit.h"
#include "../camera/Ray.h"
#include "../tools/Mat4.h"
#include "../tools/Transformation.h"
#include "Shape.h"

namespace shapes {
class Group : public Shape {
   public:
	Group(const util::Transformation& trans);
	Group(const util::Mat4& matrix);

	std::shared_ptr<cam::Hit> intersect(const cam::Ray& r);
	// protected:TODO
	void add(const std::shared_ptr<shapes::Shape>& shape);

   private:
	std::vector<std::shared_ptr<Shape>> shapeList;
	util::Transformation transform;
};
}  // namespace shapes
