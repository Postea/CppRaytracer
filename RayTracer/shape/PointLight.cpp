#define _USE_MATH_DEFINES

#include "PointLight.h"

#include "../material/BackgroundMaterial.h"
#include "math.h"

namespace shapes {
PointLight::PointLight(const util::Vec3& emission)
    : material(std::make_shared<material::BackgroundMaterial>(emission)) {
}
util::SurfacePoint PointLight::sampleLight(const cam::Hit& h) const {
	return util::SurfacePoint(util::Vec3(0), h.point().normalize(), {},
	                          material);
}
util::Vec3 PointLight::lightEmission(const util::SurfacePoint& p) const {
	return p.emission();
}
float PointLight::lightPdf(const util::SurfacePoint& p,
                           const util::Vec3& dl_out) const {
	return -std::pow(dl_out.length(), 2);
}
}  // namespace shapes
