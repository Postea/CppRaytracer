#define _USE_MATH_DEFINES

#include "CirclePlane.h"

#include "../tools/Random.h"
#include "math.h"

namespace shapes {
CirclePlane::CirclePlane(float radius, bool twofaced,
                         const std::shared_ptr<material::Material>& material)
    : radius(radius), twofaced(twofaced), material(material) {
}
std::optional<cam::Hit> CirclePlane::intersect(const cam::Ray& r) const {
	util::Vec3 n(0, 1, 0);
	util::Vec3 x0 = r.x0;
	util::Vec3 d = r.d;

	float a = util::dot(d, n);
	if (a == 0) {
		return std::nullopt;
	} else if (a > 0) {
		if (twofaced)
			n = -n;
		else
			return std::nullopt;
	}
	float t = -x0[1] / d[1];
	util::Vec3 t_hitpoint = r(t);

	if (r.in_range(t) && t_hitpoint.length() <= radius) {
		return std::optional<cam::Hit>(
		    {t_hitpoint, n, texture_coordinates(t_hitpoint), t, material});
	} else {
		return std::nullopt;
	}
}
std::pair<float, float> CirclePlane::texture_coordinates(
    const util::Vec3& pos) const {
	return std::pair<float, float>(
	    {pos.x() / radius + 0.5f, pos.z() / radius + 0.5f});
}

util::AxisAlignedBoundingBox CirclePlane::bounds() const {
	return util::AxisAlignedBoundingBox(util::Vec3(-radius, 0, -radius),
	                                    util::Vec3(radius, 0, radius));
}
util::SurfacePoint CirclePlane::sampleLight(const cam::Hit& h) const {
	// Radius of the sampled point.
	float r = std::sqrt(util::dis0to1(util::gen)) * radius;
	// Degreee of the sampled point.
	float theta = 2 * M_PI * util::dis0to1(util::gen);
	// Polar coordinates have to be converted to cartesian.
	util::Vec3 pos(r * std::cos(theta), 0, r * std::sin(theta));
	return util::SurfacePoint(pos, util::Vec3(0, 1, 0),
	                          texture_coordinates(pos), material);
	// The sampled point will be in local coordinates.
}
util::Vec3 CirclePlane::calculateLightEmission(const util::SurfacePoint& p,
                                               const util::Vec3& d) const {
	// Basically this is just the emission at a surface point. And the pdf dimms
	// the light in regard to the angle.
	// Uniform pdf of shape is 1/area, converting to pdf over solid angle is
	// pdf/(dot/length^2).
	// This is wrong. We just need the normal pdf, per area, as we do not sample
	// with regard to a direction.
	auto emission = p.emission();
	auto dot = std::max<float>(util::dot(p.normal(), d.normalize()), 0);
	auto area = M_PI * std::pow(radius, 2);
	auto pdf = 1 / area;
	return emission / pdf;
}
}  // namespace shapes
