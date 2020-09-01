#include "Material.h"

namespace material {

util::Vec3 rand_vec3() {
	return util::Vec3(dis(gen), dis(gen), dis(gen));
}

bool in_circle(const util::Vec3& v) {
	return v.length() <= 1;
}

util::Vec3 rand_vec3_in_circle(float r) {
	util::Vec3 result;
	do {
		result = rand_vec3();
	} while (!in_circle(result));
	return result * r;
}
}  // namespace material