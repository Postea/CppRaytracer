#define _USE_MATH_DEFINES

#include "RectanglePlane.h"

#include "../tools/Random.h"
#include "math.h"

namespace shapes {
RectanglePlane::RectanglePlane(
    float width, float depth, bool twofaced,
    const std::shared_ptr<material::Material>& material)
    : width(width), depth(depth), twofaced(twofaced), material(material) {
}
std::optional<cam::Hit> RectanglePlane::intersect(const cam::Ray& r) const {
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

	if (r.in_range(t) && std::abs(t_hitpoint.x()) <= width / 2 &&
	    std::abs(t_hitpoint.z()) <= depth / 2) {
		return std::optional<cam::Hit>(
		    {t_hitpoint, n, texture_coordinates(t_hitpoint), t, material});
	} else {
		return std::nullopt;
	}
}

std::pair<float, float> RectanglePlane::texture_coordinates(
    const util::Vec3& pos) const {
	return std::pair<float, float>(
	    {pos.x() / width + 0.5f, pos.z() / depth + 0.5f});
}
util::AxisAlignedBoundingBox RectanglePlane::bounds() const {
	return util::AxisAlignedBoundingBox(util::Vec3(-width / 2, 0, -depth / 2),
	                                    util::Vec3(width / 2, 0, depth / 2));
}
util::SurfacePoint RectanglePlane::sampleLight() const {
	// X coord of the sampled point.
	float x = util::disMinus1To1(util::gen) * width / 2;
	// Z coord of the sampled point.
	float z = util::disMinus1To1(util::gen) * depth / 2;
	util::Vec3 pos(x, 0, z);
	return util::SurfacePoint(pos, util::Vec3(0, 1, 0),
	                          texture_coordinates(pos), material);
	// The sampled point will be in local coordinates.
}
util::Vec3 RectanglePlane::calculateLightEmission(const util::SurfacePoint& p,
                                                  const util::Vec3& d) const {
	// Basically this is just the emission at a surface point. And the pdf dimms
	// the light in regard to the angle.
	// Uniform pdf of shape is 1/area, converting to pdf over solid angle is
	// pdf/(dot/length^2).
	auto emission = p.emission();
	auto dot = std::max<float>(util::dot(p.normal(), d.normalize()), 0);
	auto area = width * depth;
	auto pdf = std::pow(d.length(), 2) / (dot * area);
	return emission / pdf;
}
}  // namespace shapes
