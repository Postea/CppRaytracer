#pragma once

#include "../camera/Ray.h"
#include "Mat4.h"
#include "Vec3.h"

namespace util {

class AxisAlignedBoundingBox {
   public:
	AxisAlignedBoundingBox();
	AxisAlignedBoundingBox(const Vec3& min, const Vec3& max);
	// Operator
	friend std::ostream& operator<<(std::ostream& os,
	                                const AxisAlignedBoundingBox& rhs);
	AxisAlignedBoundingBox operator+(const AxisAlignedBoundingBox& rhs) const;
	AxisAlignedBoundingBox operator*(const Mat4& rhs) const;
	// Methods
	bool intersects(const cam::Ray& r) const;
	bool contains(const Vec3& v) const;
	Vec3 minBound() const;
	Vec3 maxBound() const;

   private:
	Vec3 min, max;
};
}  // namespace util