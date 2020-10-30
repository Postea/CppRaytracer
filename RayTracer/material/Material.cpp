#pragma once

#include <random>

#include "../sampling/Sampler.h"
#include "../tools/Vec3.h"

namespace material {

util::Vec3 reflection(const util::Vec3& d, const util::Vec3& n) {
	return d - (n * (2 * util::dot(n, d)));
}
}  // namespace material
