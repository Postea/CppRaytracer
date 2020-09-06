#include "Group.h"

#include "../material/Material.h"
#include "../tools/Vec3.h"

namespace shapes {
Group::Group(const util::Transformation& transform)
    : shapeList(std::vector<std::shared_ptr<Shape>>()), transform(transform) {
}
Group::Group(const util::Mat4& matrix)
    : shapeList(std::vector<std::shared_ptr<Shape>>()),
      transform(util::Transformation(matrix)) {
}

std::shared_ptr<cam::Hit> Group::intersect(const cam::Ray& r) const {
	cam::Ray imagR(transform.fromWorld.transformPoint(r.x0),
	               transform.fromWorld.transformDir(r.d), r.tmin, r.tmax,
	               r.normalize);

	std::shared_ptr<cam::Hit> result;

	for (std::shared_ptr<Shape> s : shapeList) {
		if (s->bounds().intersects(imagR)) {
			std::shared_ptr<cam::Hit> temp = s->intersect(imagR);
			if (temp != nullptr) {
				if (result == nullptr) {
					result = temp;
				} else if (result->t > temp->t) {
					result = temp;
				}
			}
		}
	}
	if (result != nullptr) {
		result = std::make_shared<cam::Hit>(
		    cam::Hit(transform.toWorld.transformPoint(result->hit),
		             transform.toWorldN.transformDir(result->n), result->t,
		             result->material));
	}
	return result;
}
util::AxisAlignedBoundingBox Group::bounds() const {
	return boundingVolume;
}
void Group::add(const std::shared_ptr<Shape>& shape) {
	shapeList.push_back(shape);
	rebuildBoundingVolume();
}
void Group::rebuildBoundingVolume() {
	util::AxisAlignedBoundingBox bb = shapeList[0]->bounds();
	for (auto shape_bb : shapeList) {
		bb = bb + shape_bb->bounds();
	}
	boundingVolume = bb;
}

}  // namespace shapes