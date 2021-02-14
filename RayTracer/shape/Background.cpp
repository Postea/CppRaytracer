#include "Background.h"

#include <limits>

namespace shapes {
Background::Background(const std::shared_ptr<material::Material>& material)
    : material(material) {
}
// TODO TEXELS
std::optional<cam::Hit> Background::intersect(const cam::Ray& r) const {
	return std::optional<cam::Hit>({r(std::numeric_limits<float>::infinity()),
	                                util::Vec3({}),
	                                {},
	                                std::numeric_limits<float>::infinity(),
	                                material});
}
// Not used
std::pair<float, float> Background::texture_coordinates(
    const util::Vec3& pos) const {
	return std::pair<float, float>({});
}

util::AxisAlignedBoundingBox Background::bounds() const {
	return util::AxisAlignedBoundingBox();
}
}  // namespace shapes