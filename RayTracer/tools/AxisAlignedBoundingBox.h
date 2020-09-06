#pragma once

#include "../camera/Ray.h"
#include "Vec3.h"

namespace util {

class AxisAlignedBoundingBox {
   public:
	AxisAlignedBoundingBox();
	AxisAlignedBoundingBox(Vec3& min, Vec3& max);
	// Operator
	AxisAlignedBoundingBox operator+(AxisAlignedBoundingBox& rhs) const;
	// Methods
	bool intersects(cam::Ray& r) const;
	bool contains(Vec3& v) const;
	Vec3 minBound() const;
	Vec3 maxBound() const;

   private:
	Vec3 min, max;
};
}  // namespace util