#define _USE_MATH_DEFINES

#include "Sphere.h"

#include <cassert>

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
		float t1 = (-b - sqrt(discrim)) / (2 * a);

		if (r.in_range(t1)) {
			util::Vec3 t1_hitpoint = r(t1);
			return std::optional<cam::Hit>({t1_hitpoint, t1_hitpoint,
			                                texture_coordinates(t1_hitpoint),
			                                t1, material});
		} else {
			float t2 = (-b + sqrt(discrim)) / (2 * a);
			if (r.in_range(t2)) {
				util::Vec3 t2_hitpoint = r(t2);
				return std::optional<cam::Hit>(
				    {t2_hitpoint, t2_hitpoint, texture_coordinates(t2_hitpoint),
				     t2, material});
			} else {
				return std::nullopt;
			}
		}
	} else {
		return std::nullopt;
	}
}

std::pair<float, float> Sphere::texture_coordinates(
    const util::Vec3& pos) const {
	float theta = std::atan2(pos.x(), pos.z());
	float phi = std::acos(pos.y() / radius);
	return std::make_pair<float, float>((float)(theta / (2 * M_PI)),
	                                    (float)(phi / (M_PI)));
}
util::Vec3 Sphere::texture_coordinates(std::pair<float, float> texel) const {
	float theta = texel.first * M_PI * 2;
	float phi = texel.second * M_PI;
	float z = radius * std::cos(theta) * std::sin(phi);
	float x = radius * std::sin(theta) * std::sin(phi);
	float y = radius * std::cos(phi);
	return {x, y, z};
}

util::AxisAlignedBoundingBox Sphere::bounds() const {
	return util::AxisAlignedBoundingBox(util::Vec3(-radius),
	                                    util::Vec3(radius));
}
}  // namespace shapes
