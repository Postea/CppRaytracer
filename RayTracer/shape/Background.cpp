#include "Background.h"
#define _USE_MATH_DEFINES
#include <limits>

#include "../material/BackgroundMaterial.h"
#include "math.h"

namespace shapes {
Background::Background(const std::shared_ptr<util::Sampler>& sampler)
    : Sphere(100, std::make_shared<material::BackgroundMaterial>(sampler)) {
}
// Thit intersect method rightly flips the normal.But the normal is never used
// for non scatter materials, so we do not flip the normal
/*
std::optional<cam::Hit> Background::intersect(const cam::Ray& r) const {
    auto hit = Sphere::intersect(r);
    if (hit)
        hit = std::optional<cam::Hit>(
            {{hit->point(), -hit->normal(), hit->texel(), hit->scalar(),
              hit->material}});
    return hit;
}*/

util::AxisAlignedBoundingBox Background::bounds() const {
	return util::AxisAlignedBoundingBox();
}
}  // namespace shapes