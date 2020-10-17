#include "LightSingleGroup.h"

#include "../material/Material.h"
#include "../tools/Vec3.h"

namespace shapes {
LightSingleGroup::LightSingleGroup(const util::Transformation& transform,
                                   std::shared_ptr<Light> light)
    : light(light), transform(transform) {
}
LightSingleGroup::LightSingleGroup(const util::Mat4& matrix,
                                   std::shared_ptr<Light> light)
    : light(light), transform(util::Transformation(matrix)) {
}

util::SurfacePoint LightSingleGroup::sampleLight() const {
	auto sample = light->sampleLight();
	auto result = util::SurfacePoint(
	    transform.toWorld.transformPoint(sample.point()),
	    transform.toWorldN.transformDir(sample.normal()), sample.material);
	return result;
}
util::Vec3 LightSingleGroup::calculateLightEmission(const util::SurfacePoint& p,
                                                    const util::Vec3& d) const {
	return light->calculateLightEmission(p, d);
}

}  // namespace shapes