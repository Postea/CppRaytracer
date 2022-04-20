#include "Checkerboard.h"

namespace material {
Checkerboard::Checkerboard(int size, const util::Vec3& c1, const util::Vec3& c2)
    : size(size), c1(c1), c2(c2) {
}

util::Vec3 Checkerboard::color(float x, float y) const {
	bool hori = (int)(x * size) % 2 == 0;
	bool verti = (int)(y * size) % 2 == 0;
	if (hori ^ verti) return c2;
	return c1;
}
}  // namespace material
