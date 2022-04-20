#include "ShapeSingleGroup.h"

#include "../material/Material.h"
#include "../tools/Vec3.h"

namespace shapes {
ShapeSingleGroup::ShapeSingleGroup(const util::Transformation& transform,
                                   const std::shared_ptr<Shape>& shape)
    : shape(shape), transform(transform) {
	bounding_box = shape->bounds() * transform.to_world;
}
ShapeSingleGroup::ShapeSingleGroup(const util::Mat4& matrix,
                                   const std::shared_ptr<Shape>& shape)
    : shape(shape), transform(util::Transformation(matrix)) {
	bounding_box = shape->bounds() * transform.to_world;
}

std::optional<cam::Hit> ShapeSingleGroup::intersect(const cam::Ray& r) const {
	cam::Ray imag_ray(transform.from_world.transform_point(r.x0),
	                  transform.from_world.transform_direction(r.d), r.tmin,
	                  r.tmax, r.normalize);

	std::optional<cam::Hit> result = std::nullopt;

	if (shape->bounds().intersects(imag_ray))
		result = shape->intersect(imag_ray);

	if (result)
		result = std::optional<cam::Hit>(
		    {transform.to_world.transform_point(result->point()),
		     transform.to_world_n.transform_direction(result->normal()),
		     result->texel(), result->scalar(), result->material});

	return result;
}

util::AxisAlignedBoundingBox ShapeSingleGroup::bounds() const {
	return bounding_box;
}

}  // namespace shapes