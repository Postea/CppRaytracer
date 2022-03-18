#include "Checkerboard.h"

namespace material {
Checkerboard::Checkerboard(int size) : size(size) {
}
util::Vec3 Checkerboard::color(float x, float y) const {
	util::Vec3 color(0);
	bool hori = (int)(x * size) % 2 == 0;
	bool verti = (int)(y * size) % 2 == 0;
	if (hori ^ verti) color = util::Vec3(1);
	return color;
}
}  // namespace material