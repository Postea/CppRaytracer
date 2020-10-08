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

std::optional<cam::Hit> Group::intersect(const cam::Ray& r) const {
	cam::Ray imagR(transform.fromWorld.transformPoint(r.x0),
	               transform.fromWorld.transformDir(r.d), r.tmin, r.tmax,
	               r.normalize);

	std::optional<cam::Hit> result = std::nullopt;

	for (std::shared_ptr<Shape> s : shapeList) {
		if (s->bounds().intersects(imagR)) {
			std::optional<cam::Hit> temp = s->intersect(imagR);
			if (temp) {
				if (!result) {
					result = temp;
				} else if (result->scalar() > temp->scalar()) {
					result = temp;
				}
			}
		}
	}

	if (result) {
		result = std::optional<cam::Hit>(
		    {transform.toWorld.transformPoint(result->point()),
		     transform.toWorldN.transformDir(result->normal()),
		     result->scalar(), result->material});
	}
	return result;
}
util::AxisAlignedBoundingBox Group::bounds() const {
	return boundingVolume;
}
void Group::add(const Group& group) {
	add(std::make_shared<Group>(group));
}
void Group::add(const SingleGroup& group) {
	add(std::make_shared<SingleGroup>(group));
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
	boundingVolume = bb * transform.toWorld;
}

}  // namespace shapes