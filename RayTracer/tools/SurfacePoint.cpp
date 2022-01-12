#include "SurfacePoint.h"

#include <limits>

#include "../camera/Ray.h"

namespace util {
SurfacePoint::SurfacePoint(const util::Vec3& point, const util::Vec3& n,
                           const std::pair<float, float>& uv,
                           const std::shared_ptr<material::Material>& material)
    : x(point), n(n), uv(uv), material(material) {
}
std::optional<cam::Ray> SurfacePoint::scattered_ray(
    const cam::Ray& inc_ray) const {
	auto d = material->scattered_d(inc_ray.d, n);
	if (d)
		return cam::Ray(x, d.value(), cam::epsilon, inc_ray.tmax, true);
	else
		return std::nullopt;
}
util::Vec3 SurfacePoint::albedo() const {
	return material->albedo(uv);
}
util::Vec3 SurfacePoint::emission() const {
	return material->emission(uv);
}
/*bool SurfacePoint::scatter(const util::Vec3& d, const util::Vec3& n) const {
    return material->scatter(d, n);
}*/
util::Vec3 SurfacePoint::point() const {
	return x;
}
util::Vec3 SurfacePoint::normal() const {
	return n;
}
std::pair<float, float> SurfacePoint::texel() const {
	return uv;
}
float SurfacePoint::calculateLightMultiplier(const util::Vec3& d_in,
                                             const util::Vec3& d_out,
                                             const util::Vec3& n) const {
	return material->calculateLightMultiplier(d_in, d_out, n);
}
}  // namespace util