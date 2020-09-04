#include "AxisAlignedBoundingBox.h"

#include <algorithm>
#include <limits>

namespace util {

// Constructor
AxisAlignedBoundingBox::AxisAlignedBoundingBox()
    : min(Vec3(-std::numeric_limits<float>::infinity())),
      max(Vec3(std::numeric_limits<float>::infinity())) {
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(Vec3& min, Vec3& max)
    : min(min), max(max) {
}
// Operator
AxisAlignedBoundingBox AxisAlignedBoundingBox::operator+(
    AxisAlignedBoundingBox& rhs) const {
	Vec3 min(std::min<float>(min.x(), rhs.min.x()),
	         std::min<float>(min.y(), rhs.min.y()),
	         std::min<float>(min.z(), rhs.min.z()));
	Vec3 max(std::max<float>(max.x(), rhs.max.x()),
	         std::max<float>(max.y(), rhs.max.y()),
	         std::max<float>(max.z(), rhs.max.z()));

	return AxisAlignedBoundingBox(min, max);
}
// Methods
// https://education.siggraph.org/static/HyperGraph/raytrace/rtinter3.htm
bool AxisAlignedBoundingBox::intersects(cam::Ray& r) const {
	float t1x = (min.x() - r.x0.x()) / r.d.x();
	float t2x = (max.x() - r.x0.x()) / r.d.x();
	float t1y = (min.y() - r.x0.y()) / r.d.y();
	float t2y = (max.y() - r.x0.y()) / r.d.y();
	float t1z = (min.z() - r.x0.z()) / r.d.z();
	float t2z = (max.z() - r.x0.z()) / r.d.z();
	float tmin =
	    std::max<float>({std::min<float>(t1x, t2x), std::min<float>(t1y, t2y),
	                     std::min<float>(t1z, t2z)});
	float tmax =
	    std::min<float>({std::max<float>(t1x, t2x), std::max<float>(t1y, t2y),
	                     std::max<float>(t1z, t2z)});

	return tmax >= tmin && r.in_range(tmax);
}

bool AxisAlignedBoundingBox::contains(Vec3& v) const {
	bool x = min.x() <= v.x() && v.x() <= max.x();
	bool y = min.y() <= v.y() && v.y() <= max.y();
	bool z = min.z() <= v.z() && v.z() <= max.z();

	return x && y && z;
}
Vec3 AxisAlignedBoundingBox::minBound() const {
	return min;
}
Vec3 AxisAlignedBoundingBox::maxBound() const {
	return max;
}
}  // namespace util