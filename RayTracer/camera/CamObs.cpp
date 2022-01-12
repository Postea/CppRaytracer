#include "CamObs.h"

#include <limits>

namespace cam {
CamObs::CamObs(const util::Mat4& mat, float theta, int width, int height)
    : mat(mat), theta(theta), width(width), height(height) {
}

// Create a new ray shooting from the camera
Ray CamObs::create(float x, float y) const {
	util::Vec3 d(x - width / 2, height / 2 - y,
	             -(width / 2) / (tan(theta / 2)));

	d = mat.transformDir(d);

	return Ray(mat.position(), d, 0, std::numeric_limits<float>::infinity(),
	           true);
}
}  // namespace cam
