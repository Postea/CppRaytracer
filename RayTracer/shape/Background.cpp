#include "Background.h"

#include <limits>

namespace shapes {
Background::Background(const std::shared_ptr<material::Material>& material)
    : material(material) {
}

std::optional<cam::Hit> Background::intersect(const cam::Ray& r) const {
	return std::optional<cam::Hit>(
	    {r(std::numeric_limits<float>::infinity()), util::Vec3(0, 0, 0),
	     std::numeric_limits<float>::infinity(), material});
}
util::AxisAlignedBoundingBox Background::bounds() const {
	return util::AxisAlignedBoundingBox();
}
}  // namespace shapes