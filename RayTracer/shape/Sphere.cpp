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
// TODO: Rework this function to be yours. THIS IS COPIED!!!! HIT IS A WRONG
// RETURN VALUE hit's n is wrong and bad
util::SurfacePoint Sphere::sampleLight() const {
	float u[2] = {(float)util::dis(util::gen), (float)util::dis(util::gen)};
	float z = u[0];
	float r = std::sqrt(std::max((float)0, (float)1. - z * z));
	float phi = 2 * M_PI * u[1];
	util::Vec3 point(r * std::cos(phi), r * std::sin(phi), z);
	return util::SurfacePoint(point, point, material);
}
}  // namespace shapes
