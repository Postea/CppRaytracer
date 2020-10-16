#include "Random.h"

namespace util {

Vec3 rand_vec3() {
	return Vec3(disMinus1To1(gen), disMinus1To1(gen), disMinus1To1(gen));
}

bool in_circle(const Vec3& v) {
	return v.length() <= 1;
}

Vec3 rand_vec3_in_circle(float r) {
	Vec3 result;
	do {
		result = rand_vec3();
	} while (!in_circle(result));
	return result * r;
}
}  // namespace util