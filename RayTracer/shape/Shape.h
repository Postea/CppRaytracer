#pragma once

#include <optional>

#include "../camera/Hit.h"
#include "../camera/Ray.h"
#include "../tools/AxisAlignedBoundingBox.h"

namespace shapes {
class Shape {
   public:
	virtual std::optional<cam::Hit> intersect(const cam::Ray& r) const = 0;
	virtual std::pair<float, float> texture_coordinates(
	    const util::Vec3& pos) const = 0;
	virtual util::AxisAlignedBoundingBox bounds() const = 0;
};
}  // namespace shapes