#include "Group.h"

#include <cassert>

#include "../material/Material.h"
#include "../tools/Vec3.h"

namespace shapes {
Group::Group(const util::Transformation& transform, bool rebuild_bb)
    : shape_list(std::vector<std::shared_ptr<Shape>>()),
      transform(transform),
      rebuild_bb(rebuild_bb) {
}
Group::Group(const util::Mat4& matrix, bool rebuild_bb)
    : shape_list(std::vector<std::shared_ptr<Shape>>()),
      transform(util::Transformation(matrix)),
      rebuild_bb(rebuild_bb) {
}

std::optional<cam::Hit> Group::intersect(const cam::Ray& r) const {
	cam::Ray imag_ray(transform.from_world.transform_point(r.x0),
	                  transform.from_world.transform_direction(r.d), r.tmin,
	                  r.tmax, r.normalize);

	std::optional<cam::Hit> result = std::nullopt;

	for (const auto& s : shape_list) {
		if (s->bounds().intersects(imag_ray)) {
			std::optional<cam::Hit> temp = s->intersect(imag_ray);
			if (temp) {
				if (imag_ray.in_range(temp->scalar())) {
					if (!result) {
						result = temp;
					} else if (result->scalar() > temp->scalar()) {
						result = temp;
					}
				}
			}
		}
	}

	if (result) {
		result = std::optional<cam::Hit>(
		    {transform.to_world.transform_point(result->point()),
		     transform.to_world_n.transform_direction(result->normal()),
		     result->texel(), result->scalar(), result->material});
	}
	return result;
}

util::AxisAlignedBoundingBox Group::bounds() const {
	return bounding_box;
}

void Group::set_bounds(const util::AxisAlignedBoundingBox& bb) {
	bounding_box = bb;
}

void Group::add(const Group& group) {
	add(std::make_shared<Group>(group));
}

void Group::add(const ShapeSingleGroup& group) {
	add(std::make_shared<ShapeSingleGroup>(group));
}

void Group::add(const std::shared_ptr<Shape>& shape) {
	shape_list.push_back(shape);
	if (rebuild_bb) rebuild_boundingbox();
}

void Group::rebuild_boundingbox() {
	util::AxisAlignedBoundingBox bb = shape_list[0]->bounds();
	for (auto shape_bb : shape_list) {
		bb = bb + shape_bb->bounds();
	}
	bounding_box = bb * transform.to_world;
}

}  // namespace shapes