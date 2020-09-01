#include "Constant.h"

namespace material {
Constant::Constant(const util::Vec3& albedo) : albedo(albedo) {
}
util::Vec3 Constant::color(float x, float y) const {
	return albedo;
}
}  // namespace material
