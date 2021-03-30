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

util::SurfacePoint LightSingleGroup::sampleLight(const cam::Hit& h) const {
	cam::Hit xx(transform.fromWorld.transformPoint(h.point()),
	            transform.fromWorldN.transformDir(h.normal()), h.texel(),
	            h.scalar(), h.material);
	auto sample = light->sampleLight(xx);
	auto result =
	    util::SurfacePoint(transform.toWorld.transformPoint(sample.point()),
	                       transform.toWorldN.transformDir(sample.normal()),
	                       sample.texel(), sample.material);
	return result;
}
/*
std::pair<util::Vec3, float> LightSingleGroup::calculateLightEmission(
    const util::SurfacePoint& p, const util::Vec3& d) const {
    return light->calculateLightEmission(p, d);
}*/

util::Vec3 LightSingleGroup::lightEmission(const util::SurfacePoint& p) const {
	return light->lightEmission(p);
}

float LightSingleGroup::lightPdf(const util::SurfacePoint& p,
                                 const util::Vec3& dl_out) const {
	return light->lightPdf(p, dl_out);
}

}  // namespace shapes