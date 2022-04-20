#define _USE_MATH_DEFINES

#include "../tools/Random.h"
#include "Rectangle.h"
#include "math.h"

namespace shapes {
Rectangle::Rectangle(float width, float depth, bool two_faced,
                     const std::shared_ptr<material::Material>& material)
    : width(width), depth(depth), two_faced(two_faced), material(material) {
}
std::optional<cam::Hit> Rectangle::intersect(const cam::Ray& r) const {
	util::Vec3 n(0, 1, 0);
	util::Vec3 x0 = r.x0;
	util::Vec3 d = r.d;

	float a = util::dot(d, n);
	if (a == 0) return std::nullopt;

	if (a > 0) {
		if (two_faced)
			n = -n;
		else
			return std::nullopt;
	}
	float t = -x0[1] / d[1];
	util::Vec3 t_hitpoint = r(t);

	if (r.in_range(t) && std::abs(t_hitpoint.x()) <= width / 2 &&
	    std::abs(t_hitpoint.z()) <= depth / 2)
		return std::make_optional<cam::Hit>(
		    {t_hitpoint, n, texture_coordinates(t_hitpoint), t, material});

	return std::nullopt;
}

std::pair<float, float> Rectangle::texture_coordinates(
    const util::Vec3& pos) const {
	return std::pair<float, float>(
	    {pos.x() / width + 0.5f, pos.z() / depth + 0.5f});
}
util::Vec3 Rectangle::texture_coordinates(
    const std::pair<float, float>& texel) const {
	return {(texel.first - 0.5f) * width, 0, (texel.second - 0.5f) * depth};
}

util::AxisAlignedBoundingBox Rectangle::bounds() const {
	return util::AxisAlignedBoundingBox(util::Vec3(-width / 2, 0, -depth / 2),
	                                    util::Vec3(width / 2, 0, depth / 2));
}

util::SurfacePoint Rectangle::sample_light(const cam::Hit& h) const {
	auto uv = material->sample_emission_profile();
	util::Vec3 point = texture_coordinates(uv);
	return util::SurfacePoint(point, util::Vec3(0, 1, 0), uv, material);
}

util::Vec3 Rectangle::light_emission(const util::SurfacePoint& p) const {
	return p.emission();
}

float Rectangle::light_pdf(const util::SurfacePoint& p,
                           const util::Vec3& dl_out) const {
	auto dot = util::dot(p.normal(), dl_out.normalize());
	if (two_faced)
		dot = std::abs(dot);
	else
		dot = std::max<float>(dot, 0);
	auto area = width * depth;
	auto uv = p.texel();
	float pdf = material->emission_pdf(uv.first, uv.second).value_or(1) / area;
	pdf = pdf / (dot / dl_out.length());
	return pdf;
}
}  // namespace shapes
