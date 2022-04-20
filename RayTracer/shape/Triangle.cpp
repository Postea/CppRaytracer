#pragma once

#include "Triangle.h"

#include "../tools/Random.h"
#include "math.h"

namespace shapes {
Triangle::Triangle(const util::Vertex& p1, const util::Vertex& p2,
                   const util::Vertex& p3,
                   const std::shared_ptr<material::Material>& material)
    : p1(p1), p2(p2), p3(p3), material(material) {
	recalculate_bb();
}

// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
std::optional<cam::Hit> Triangle::intersect(const cam::Ray& r) const {
	util::Vec3 e1 = p2.position - p1.position;
	util::Vec3 e2 = p3.position - p1.position;
	util::Vec3 pvec = util::cross(r.d, e2);
	float det = util::dot(e1, pvec);
	if (det < cam::epsilon) return std::nullopt;

	util::Vec3 tvec = r.x0 - p1.position;
	float u = util::dot(tvec, pvec) / det;
	if (u < 0 || u > 1) return std::nullopt;

	util::Vec3 qvec = util::cross(tvec, e1);
	float v = util::dot(r.d, qvec) / det;
	if (v < 0 || u + v > 1) return std::nullopt;

	float t = util::dot(e2, qvec) / det;
	util::Vec3 hit = r.x0 + r.d * t;

	// float w = 1 - u - v;

	auto cross_normal =
	    util::cross(p2.position - p1.position, p3.position - p1.position)
	        .normalize();

	return std::optional<cam::Hit>(
	    cam::Hit(hit, cross_normal, {}, t, material));
}

util::AxisAlignedBoundingBox Triangle::bounds() const {
	return bb;
}

void Triangle::recalculate_bb() {
	const util::Vec3 xx = p1.position;
	const util::Vec3 yy = p2.position;
	const util::Vec3 zz = p3.position;
	const util::Vec3 min_bound =
	    util::Vec3(std::min<float>({xx.x(), yy.x(), zz.x()}),
	               std::min<float>({xx.y(), yy.y(), zz.y()}),
	               std::min<float>({xx.z(), yy.z(), zz.z()}));
	const util::Vec3 max_bound =
	    util::Vec3(std::max<float>({xx.x(), yy.x(), zz.x()}),
	               std::max<float>({xx.y(), yy.y(), zz.y()}),
	               std::max<float>({xx.z(), yy.z(), zz.z()}));
	bb = util::AxisAlignedBoundingBox(min_bound, max_bound);
}

}  // namespace shapes
