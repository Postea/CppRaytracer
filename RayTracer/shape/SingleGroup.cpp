#include "SingleGroup.h"

#include "../material/Material.h"
#include "../tools/Vec3.h"

namespace shapes {
SingleGroup::SingleGroup(const util::Transformation& transform,
                         std::shared_ptr<LightShape> shape)
    : shape(shape), transform(transform) {
	boundingVolume = shape->bounds() * transform.toWorld;
}
SingleGroup::SingleGroup(const util::Mat4& matrix,
                         std::shared_ptr<LightShape> shape)
    : shape(shape), transform(util::Transformation(matrix)) {
	boundingVolume = shape->bounds() * transform.toWorld;
}

std::optional<cam::Hit> SingleGroup::intersect(const cam::Ray& r) const {
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
		     transform.toWorldN.transformDir(result->normal()),
		     result->scalar(), result->material});
	}
	return result;
}
util::AxisAlignedBoundingBox SingleGroup::bounds() const {
	return boundingVolume;
}

util::SurfacePoint SingleGroup::sampleLight() const {
	return shape->sampleLight();
}

}  // namespace shapes