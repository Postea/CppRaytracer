#include "Hit.h"

#include <limits>

namespace cam {
Hit::Hit(const util::Vec3& hit, const util::Vec3& n, float t,
         const std::shared_ptr<material::Material>& material)
    : util::SurfacePoint(hit, n, material), t(t) {
}

float Hit::scalar() const {
	return t;
}
}  // namespace cam