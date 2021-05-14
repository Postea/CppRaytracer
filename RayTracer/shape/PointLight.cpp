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
/*std::pair<util::Vec3, float> PointLight::calculateLightEmission(
    const util::SurfacePoint& p, const util::Vec3& d) const {
    // Basically this is just the emission at a surface point. And the pdf dimms
    // the light in regard to the angle.
    auto emission = p.emission();
    // Illegally do not divide by area
    // auto area = M_PI * std::pow(radius, 2);
    auto pdf = std::pow(d.length(), 2);
    return {emission, pdf};
}*/
util::Vec3 PointLight::lightEmission(const util::SurfacePoint& p) const {
	return p.emission();
}
float PointLight::lightPdf(const util::SurfacePoint& p,
                           const util::Vec3& dl_out) const {
	return -std::pow(dl_out.length(), 2);
}
}  // namespace shapes
