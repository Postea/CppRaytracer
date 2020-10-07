#include "SurfacePoint.h"

#include <limits>

#include "../camera/Ray.h"

namespace util {
SurfacePoint::SurfacePoint(const util::Vec3& point, const util::Vec3& n,
                           const std::shared_ptr<material::Material>& material)
    : x(point), n(n), material(material) {
}
cam::Ray SurfacePoint::scattered_ray(const cam::Ray& inc_ray) const {
	return cam::Ray(x, material->scattered_d(inc_ray.d, n),
	                std::numeric_limits<float>::epsilon(), inc_ray.tmax, true);
}
util::Vec3 SurfacePoint::albedo() const {
	return material->albedo(0, 0);
}
util::Vec3 SurfacePoint::emission() const {
	return material->emission(0, 0);
}
bool SurfacePoint::scatter() const {
	return material->scatter();
}
util::Vec3 SurfacePoint::point() const {
	return x;
}
util::Vec3 SurfacePoint::normal() const {
	return n;
}
}  // namespace util