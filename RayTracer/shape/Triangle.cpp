#pragma once

#include "Triangle.h"

#include "../tools/Random.h"
#include "math.h"

namespace shapes {
Triangle::Triangle(util::Vertex p1, util::Vertex p2, util::Vertex p3,
                   const std::shared_ptr<material::Material>& material)
    : p1(p1), p2(p2), p3(p3), material(material) {
	recalculateBB();
}
std::optional<cam::Hit> Triangle::intersect(const cam::Ray& r) const {
	// std::cout << "In tri intersect" << std::endl;
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

	float w = 1 - u - v;
	/*
	std::cout << p1().normal << std::endl;
	std::cout << p2().normal << std::endl;
	std::cout << p3().normal << std::endl;
	std::cout << u << std::endl;
	std::cout << v << std::endl;
	std::cout << w << std::endl;*/
	// auto bary_normal =
	//    (u * p1().normal + v * p2().normal + w * p3().normal).normalize();
	auto cross_normal =
	    util::cross(p2.position - p1.position, p3.position - p1.position)
	        .normalize();
	// if (util::dot(bary_normal, cross_normal) < 0)
	//	std::cout << "Hm" << std::endl;
	return std::optional<cam::Hit>(cam::Hit(hit, cross_normal, t, material));
}

util::AxisAlignedBoundingBox Triangle::bounds() const {
	// std::cout << "In tri bounds" << std::endl;
	return bb;
}
// TODO
util::SurfacePoint Triangle::sampleLight() const {
	// X coord of the sampled point.
	float x = util::disMinus1To1(util::gen) * 1 / 2;
	// Z coord of the sampled point.
	float z = util::disMinus1To1(util::gen) * 1 / 2;
	return util::SurfacePoint(util::Vec3(x, 0, z), util::Vec3(0, 1, 0),
	                          material);
	// The sampled point will be in local coordinates.
}
// TODO
util::Vec3 Triangle::calculateLightEmission(const util::SurfacePoint& p,
                                            const util::Vec3& d) const {
	// Basically this is just the emission at a surface point. And the pdf dimms
	// the light in regard to the angle.
	// Uniform pdf of shape is 1/area, converting to pdf over solid angle is
	// pdf/(dot/length^2).
	auto emission = p.emission();
	auto dot = std::max<float>(util::dot(p.normal(), d.normalize()), 0);
	auto area = 1;
	auto pdf = std::pow(d.length(), 2) / (dot * area);
	return emission / pdf;
}

void Triangle::recalculateBB() {
	const util::Vec3 xx = p1.position;
	const util::Vec3 yy = p2.position;
	const util::Vec3 zz = p3.position;
	const util::Vec3 minBound =
	    util::Vec3(std::min<float>({xx.x(), yy.x(), zz.x()}),
	               std::min<float>({xx.y(), yy.y(), zz.y()}),
	               std::min<float>({xx.z(), yy.z(), zz.z()}));
	const util::Vec3 maxBound =
	    util::Vec3(std::max<float>({xx.x(), yy.x(), zz.x()}),
	               std::max<float>({xx.y(), yy.y(), zz.y()}),
	               std::max<float>({xx.z(), yy.z(), zz.z()}));
	bb = util::AxisAlignedBoundingBox(minBound, maxBound);
}

}  // namespace shapes
