#define _USE_MATH_DEFINES

#include "Sphere.h"

#include "math.h"

namespace shapes {
Sphere::Sphere(float radius,
               const std::shared_ptr<material::Material>& material)
    : radius(radius), material(material) {
}
std::shared_ptr<cam::Hit> Sphere::intersect(const cam::Ray& r) const {
	util::Vec3 d = r.d;
	util::Vec3 x0 = r.x0;
	float a = util::dot(d, d);
	float b = 2 * util::dot(x0, d);
	float c = util::dot(x0, x0) - (radius * radius);
	float discrim = b * b - 4 * a * c;

	if (discrim >= 0) {
		double t1 = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);

		if (r.in_range(t1)) {
			util::Vec3 t1HitPoint = r(t1);
			double theta = acos(t1HitPoint.y() / radius);
			double phi = M_PI + atan2(t1HitPoint.x(), t1HitPoint.z());
			return std::make_shared<cam::Hit>(
			    cam::Hit(t1HitPoint, t1HitPoint, t1, material));
		} else {
			double t2 = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
			if (r.in_range(t2)) {
				util::Vec3 t2HitPoint = r(t2);
				double theta = acos(t2HitPoint.y() / radius);
				double phi = M_PI + atan2(t2HitPoint.x(), t2HitPoint.z());
				return std::make_shared<cam::Hit>(
				    cam::Hit(t2HitPoint, t2HitPoint, t2, material));
			} else {
				return nullptr;
			}
		}
	} else {
		return nullptr;
	}
}
}  // namespace shapes
