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
	bool contains(const AxisAlignedBoundingBox& bb) const;
	bool partially_contains(const AxisAlignedBoundingBox& bb) const;
	float size() const;
	Vec3 center() const;
	Vec3 min_bound() const;
	Vec3 max_bound() const;

   private:
	Vec3 min, max;
	void orientate();
};

// From cgg Abgaben of Prof. Tramberend
std::array<AxisAlignedBoundingBox, 2> split_bb(
    const AxisAlignedBoundingBox& box);
}  // namespace util