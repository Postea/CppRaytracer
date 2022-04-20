#define _USE_MATH_DEFINES

#include "Disk.h"

#include "../tools/Random.h"
#include "math.h"

namespace shapes {
Disk::Disk(float radius, bool twofaced,
           const std::shared_ptr<material::Material>& material)
    : radius(radius), twofaced(twofaced), material(material) {
}

std::optional<cam::Hit> Disk::intersect(const cam::Ray& r) const {
	util::Vec3 n(0, 1, 0);
	util::Vec3 x0 = r.x0;
	util::Vec3 d = r.d;

	float a = util::dot(d, n);
	if (a == 0) return std::nullopt;

	if (a > 0) {
		if (twofaced)
			n = -n;
		else
			return std::nullopt;
	}
	float t = -x0[1] / d[1];
	util::Vec3 t_hitpoint = r(t);

	if (r.in_range(t) && t_hitpoint.length() <= radius)
		return std::optional<cam::Hit>(
		    {t_hitpoint, n, texture_coordinates(t_hitpoint), t, material});

	return std::nullopt;
}

std::pair<float, float> Disk::texture_coordinates(const util::Vec3& pos) const {
	float x = pos.x() / radius;
	float z = pos.z() / radius;
	float r = std::sqrt(x * x + z * z);
	float theta = std::atan2(z, x);
	return std::pair<float, float>(r * r, (float)(theta / (2.0f * M_PI)));
}

util::AxisAlignedBoundingBox Disk::bounds() const {
	return util::AxisAlignedBoundingBox(util::Vec3(-radius, 0, -radius),
	                                    util::Vec3(radius, 0, radius));
}
}  // namespace shapes
