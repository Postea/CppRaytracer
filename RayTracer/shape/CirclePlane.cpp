#define _USE_MATH_DEFINES

#include "CirclePlane.h"

#include "../tools/Random.h"
#include "math.h"

namespace shapes {
CirclePlane::CirclePlane(float radius,
                         const std::shared_ptr<material::Material>& material)
    : radius(radius), material(material) {
}
std::optional<cam::Hit> CirclePlane::intersect(const cam::Ray& r) const {
	util::Vec3 n(0, 1, 0);
	util::Vec3 x0 = r.x0;
	util::Vec3 d = r.d;

	if (util::dot(d, n) > 0) {
		n = n * -1;
	}
	float a = util::dot(d, n);
	if (a == 0) {
		return std::nullopt;
	} else {
		float t = -x0[1] / d[1];
		util::Vec3 t_hitpoint = r(t);

		if (r.in_range(t) && t_hitpoint.length() <= radius) {
			return std::optional<cam::Hit>({t_hitpoint, n, t, material});
		} else {
			return std::nullopt;
		}
	}
}
util::AxisAlignedBoundingBox CirclePlane::bounds() const {
	return util::AxisAlignedBoundingBox(util::Vec3(-radius, 0, -radius),
	                                    util::Vec3(radius, 0, radius));
}
// THIS IS COPIED AND BAD TODO!!!!
util::SurfacePoint CirclePlane::sampleLight() const {
	float u[2] = {(float)util::dis0to1(util::gen),
	              (float)util::dis0to1(util::gen)};
	float r = std::sqrt(u[0]) * radius;
	float theta = 2 * M_PI * u[1];
	return util::SurfacePoint(
	    util::Vec3(r * std::cos(theta), 0, r * std::sin(theta)),
	    util::Vec3(0, -1, 0), material);
}
util::Vec3 CirclePlane::calculateLightEmission(const util::SurfacePoint& p,
                                               const util::Vec3& d) const {
	// Diffus ist hier im dot-product eingebettet
	return p.albedo() * (p.emission() * util::dot(p.normal(), d.normalize())) /
	       std::pow(d.length(), 2);
}
}  // namespace shapes
