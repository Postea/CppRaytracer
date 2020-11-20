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

	float a = util::dot(d, n);
	if (a == 0) {
		return std::nullopt;
	} else if (a < 0) {
		float t = -x0[1] / d[1];
		util::Vec3 t_hitpoint = r(t);

		if (r.in_range(t) && t_hitpoint.length() <= radius) {
			return std::optional<cam::Hit>({t_hitpoint, n, t, material});
		} else {
			return std::nullopt;
		}
	} else {
		return std::nullopt;
	}
}
util::AxisAlignedBoundingBox CirclePlane::bounds() const {
	return util::AxisAlignedBoundingBox(util::Vec3(-radius, 0, -radius),
	                                    util::Vec3(radius, 0, radius));
}
util::SurfacePoint CirclePlane::sampleLight() const {
	// Radius of the sampled point
	float r = std::sqrt(util::dis0to1(util::gen)) * radius;
	// Degreee of the sampled point
	float theta = 2 * M_PI * util::dis0to1(util::gen);
	// Polar coordinates have to be converted to cartesian
	return util::SurfacePoint(
	    util::Vec3(r * std::cos(theta), 0, r * std::sin(theta)),
	    util::Vec3(0, 1, 0), material);
	// The sampled point will be in local coordinates.
}
util::Vec3 CirclePlane::calculateLightEmission(const util::SurfacePoint& p,
                                               const util::Vec3& d) const {
	// Basically this is just the emission at a surface point. And the pdf dimms
	// the light in regard to the angle
	auto emission = p.emission();
	auto dot = std::max<float>(util::dot(p.normal(), d.normalize()), 0);
	auto area = M_PI * std::pow(radius, 2);
	auto pdf = std::pow(d.length(), 2) / (dot * area);
	return emission / pdf;
}
}  // namespace shapes
