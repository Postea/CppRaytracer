#include "Background.h"

#include <limits>

namespace shapes {
Background::Background(const std::shared_ptr<material::Material>& material)
    : material(material) {
}

std::shared_ptr<cam::Hit> Background::intersect(const cam::Ray& r) const {
	return std::make_shared<cam::Hit>(
	    cam::Hit(r(std::numeric_limits<float>::infinity()), util::Vec3(0, 0, 0),
	             std::numeric_limits<float>::infinity(), material));
}
}  // namespace shapes