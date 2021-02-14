#include "ShapeSingleGroup.h"

#include "../material/Material.h"
#include "../tools/Vec3.h"

namespace shapes {
ShapeSingleGroup::ShapeSingleGroup(const util::Transformation& transform,
                                   std::shared_ptr<Shape> shape)
    : shape(shape), transform(transform) {
	boundingVolume = shape->bounds() * transform.toWorld;
}
ShapeSingleGroup::ShapeSingleGroup(const util::Mat4& matrix,
                                   std::shared_ptr<Shape> shape)
    : shape(shape), transform(util::Transformation(matrix)) {
	boundingVolume = shape->bounds() * transform.toWorld;
}
std::optional<cam::Hit> ShapeSingleGroup::intersect(const cam::Ray& r) const {
	cam::Ray imagR(transform.fromWorld.transformPoint(r.x0),
	               transform.fromWorld.transformDir(r.d), r.tmin, r.tmax,
	               r.normalize);

	std::optional<cam::Hit> result = std::nullopt;

	if (shape->bounds().intersects(imagR)) {
		result = shape->intersect(imagR);
	}

	if (result) {
		result = std::optional<cam::Hit>(
		    {transform.toWorld.transformPoint(result->point()),
		     transform.toWorldN.transformDir(result->normal()), result->texel(),
		     result->scalar(), result->material});
	}
	return result;
}
std::pair<float, float> ShapeSingleGroup::texture_coordinates(
    const util::Vec3& pos) const {
	return std::pair<float, float>({});
}
util::AxisAlignedBoundingBox ShapeSingleGroup::bounds() const {
	return boundingVolume;
}

}  // namespace shapes