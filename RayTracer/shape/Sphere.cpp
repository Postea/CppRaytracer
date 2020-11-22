#define _USE_MATH_DEFINES

#include "Sphere.h"

#include "../tools/Random.h"
#include "math.h"

namespace shapes {
Sphere::Sphere(float radius,
               const std::shared_ptr<material::Material>& material)
    : radius(radius), material(material) {
}
std::optional<cam::Hit> Sphere::intersect(const cam::Ray& r) const {
	util::Vec3 d = r.d;
	util::Vec3 x0 = r.x0;
	float a = util::dot(d, d);
	float b = 2 * util::dot(x0, d);
	float c = util::dot(x0, x0) - (radius * radius);
	float discrim = b * b - 4 * a * c;

	if (discrim >= 0) {
		float t1 = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);

		if (r.in_range(t1)) {
			util::Vec3 t1HitPoint = r(t1);
			float theta = acos(t1HitPoint.y() / radius);
			float phi = M_PI + atan2(t1HitPoint.x(), t1HitPoint.z());
			return std::optional<cam::Hit>(
			    {t1HitPoint, t1HitPoint, t1, material});
		} else {
			float t2 = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
			if (r.in_range(t2)) {
				util::Vec3 t2HitPoint = r(t2);
				float theta = acos(t2HitPoint.y() / radius);
				float phi = M_PI + atan2(t2HitPoint.x(), t2HitPoint.z());
				return std::optional<cam::Hit>(
				    {t2HitPoint, t2HitPoint, t2, material});
			} else {
				return std::nullopt;
			}
		}
	} else {
		return std::nullopt;
	}
}
util::AxisAlignedBoundingBox Sphere::bounds() const {
	return util::AxisAlignedBoundingBox(util::Vec3(-radius),
	                                    util::Vec3(radius));
}
util::SurfacePoint Sphere::sampleLight() const {
	// Theta of sampled point.
	float theta = 2 * M_PI * util::dis0to1(util::gen);
	// Phi of the sampled point.
	float phi = std::acos((2 * util::dis0to1(util::gen) - 1));
	// Convert from polar coordinates to cartesian.
	util::Vec3 point(radius * std::cos(theta) * std::sin(phi),
	                 radius * std::sin(theta) * std::sin(phi),
	                 radius * std::cos(phi));
	return util::SurfacePoint(point, point.normalize(), material);
}
util::Vec3 Sphere::calculateLightEmission(const util::SurfacePoint& p,
                                          const util::Vec3& d) const {
	// Basically this is just the emission at a surface point. And the pdf dimms
	// the light in regard to the angle.
	// Uniform pdf of shape is 1/area, converting to pdf over solid angle is
	// pdf/(dot/length^2).
	auto emission = p.emission();
	auto dot = std::max<float>(util::dot(p.normal(), d.normalize()), 0);
	auto area = 4 * M_PI * std::pow(radius, 2);
	auto pdf = std::pow(d.length(), 2) / (dot * area);
	return emission / pdf;
}
}  // namespace shapes
