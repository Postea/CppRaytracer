#include "AxisAlignedBoundingBox.h"

#include <algorithm>
#include <limits>

namespace util {

// Constructor
AxisAlignedBoundingBox::AxisAlignedBoundingBox()
    : min(Vec3(-std::numeric_limits<float>::infinity())),
      max(Vec3(std::numeric_limits<float>::infinity())) {
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const Vec3& min, const Vec3& max)
    : min(min), max(max) {
}
// Operator
AxisAlignedBoundingBox AxisAlignedBoundingBox::operator+(
    const AxisAlignedBoundingBox& rhs) const {
	Vec3 min(std::min<float>(min.x(), rhs.min.x()),
	         std::min<float>(min.y(), rhs.min.y()),
	         std::min<float>(min.z(), rhs.min.z()));
	Vec3 max(std::max<float>(max.x(), rhs.max.x()),
	         std::max<float>(max.y(), rhs.max.y()),
	         std::max<float>(max.z(), rhs.max.z()));

	return AxisAlignedBoundingBox(min, max);
}
AxisAlignedBoundingBox AxisAlignedBoundingBox::operator*(
    const Mat4& rhs) const {
	AxisAlignedBoundingBox result(
	    Vec3(std::numeric_limits<float>::infinity()),
	    Vec3(-std::numeric_limits<float>::infinity()));
	Vec3 v1 = rhs.transformPoint(Vec3(min.x(), min.y(), min.z()));
	Vec3 v2 = rhs.transformPoint(Vec3(min.x(), min.y(), max.z()));
	Vec3 v3 = rhs.transformPoint(Vec3(min.x(), max.y(), min.z()));
	Vec3 v4 = rhs.transformPoint(Vec3(min.x(), min.y(), max.z()));
	Vec3 v5 = rhs.transformPoint(Vec3(max.x(), min.y(), min.z()));
	Vec3 v6 = rhs.transformPoint(Vec3(min.x(), min.y(), max.z()));
	Vec3 v7 = rhs.transformPoint(Vec3(min.x(), max.y(), min.z()));
	Vec3 v8 = rhs.transformPoint(Vec3(min.x(), max.y(), max.z()));
	float minX = std::min<float>(
	    {v1.x(), v2.x(), v3.x(), v4.x(), v5.x(), v6.x(), v7.x(), v8.x()});
	float minY = std::min<float>(
	    {v1.y(), v2.y(), v3.y(), v4.y(), v5.y(), v6.y(), v7.y(), v8.y()});
	float minZ = std::min<float>(
	    {v1.z(), v2.z(), v3.z(), v4.z(), v5.z(), v6.z(), v7.z(), v8.z()});
	float maxX = std::max<float>(
	    {v1.x(), v2.x(), v3.x(), v4.x(), v5.x(), v6.x(), v7.x(), v8.x()});
	float maxY = std::max<float>(
	    {v1.y(), v2.y(), v3.y(), v4.y(), v5.y(), v6.y(), v7.y(), v8.y()});
	float maxZ = std::max<float>(
	    {v1.z(), v2.z(), v3.z(), v4.z(), v5.z(), v6.z(), v7.z(), v8.z()});

	return AxisAlignedBoundingBox(Vec3(minX, minY, minZ),
	                              Vec3(maxX, maxY, maxZ));
}
// Methods
// https://education.siggraph.org/static/HyperGraph/raytrace/rtinter3.htm
bool AxisAlignedBoundingBox::intersects(const cam::Ray& r) const {
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

bool AxisAlignedBoundingBox::contains(const Vec3& v) const {
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