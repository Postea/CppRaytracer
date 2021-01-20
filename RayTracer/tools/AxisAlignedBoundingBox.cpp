#include "AxisAlignedBoundingBox.h"

#include <algorithm>
#include <array>
#include <limits>

namespace util {

// Constructor
AxisAlignedBoundingBox::AxisAlignedBoundingBox()
    : min(Vec3(-std::numeric_limits<float>::infinity())),
      max(Vec3(std::numeric_limits<float>::infinity())) {
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const Vec3& min, const Vec3& max)
    : min(min), max(max) {
	orientate();
}
// Operator

std::ostream& operator<<(std::ostream& os, const AxisAlignedBoundingBox& rhs) {
	os << rhs.min << " | " << rhs.max;
	return os;
}
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
	if (contains(r(r.tmin))) return true;
	if (contains(r(r.tmax))) return true;

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
// This Method is not entirely correct. It only checks if the corners of this bb
// is inside the arguments bb. This should be enough for the splitting algorithm
// to create a hierarchy. Theoretically there are overlapping bbs, where no
// corner is inside the others.
bool AxisAlignedBoundingBox::partiallyContains(
    const AxisAlignedBoundingBox bb) {
	std::array<Vec3, 8> vertices = {
	    Vec3(min.x(), min.y(), min.z()), Vec3(min.x(), min.y(), max.z()),
	    Vec3(min.x(), max.y(), min.z()), Vec3(min.x(), max.y(), max.z()),
	    Vec3(max.x(), min.y(), min.z()), Vec3(max.x(), min.y(), max.z()),
	    Vec3(max.x(), max.y(), min.z()), Vec3(max.x(), max.y(), max.z())};
	for (Vec3 v : vertices)
		if (bb.contains(v)) return true;
	return false;
}
Vec3 AxisAlignedBoundingBox::center() const {
	return (max + min) / 2;
}
Vec3 AxisAlignedBoundingBox::minBound() const {
	return min;
}
Vec3 AxisAlignedBoundingBox::maxBound() const {
	return max;
}
void AxisAlignedBoundingBox::orientate() {
	min = util::Vec3(std::min<float>(min.x(), max.x()),
	                 std::min<float>(min.y(), max.y()),
	                 std::min<float>(min.z(), max.z()));
	max = util::Vec3(std::max<float>(min.x(), max.x()),
	                 std::max<float>(min.y(), max.y()),
	                 std::max<float>(min.z(), max.z()));
}

std::array<AxisAlignedBoundingBox, 2> splitAABB(AxisAlignedBoundingBox box) {
	util::Vec3 size2 = (box.maxBound() - box.minBound()) / 2;
	AxisAlignedBoundingBox left;
	if (size2.x() >= size2.y() && size2.x() >= size2.z()) {
		left = AxisAlignedBoundingBox(
		    box.minBound(), util::Vec3(box.minBound().x() + size2.x(),
		                               box.maxBound().y(), box.maxBound().z()));
	} else if (size2.y() >= size2.x() && size2.y() >= size2.z()) {
		left = AxisAlignedBoundingBox(
		    box.minBound(),
		    util::Vec3(box.maxBound().x(), box.minBound().y() + size2.y(),
		               box.maxBound().z()));
	} else {
		left = AxisAlignedBoundingBox(
		    box.minBound(), util::Vec3(box.maxBound().x(), box.maxBound().y(),
		                               box.minBound().z() + size2.z()));
	}
	AxisAlignedBoundingBox right;
	if (size2.x() >= size2.y() && size2.x() >= size2.z()) {
		right = AxisAlignedBoundingBox(
		    util::Vec3(box.minBound().x() + size2.x(), box.minBound().y(),
		               box.minBound().z()),
		    box.maxBound());
	} else if (size2.y() >= size2.x() && size2.y() >= size2.z()) {
		right = AxisAlignedBoundingBox(
		    util::Vec3(box.minBound().x(), box.minBound().y() + size2.y(),
		               box.minBound().z()),
		    box.maxBound());
	} else {
		right = AxisAlignedBoundingBox(
		    util::Vec3(box.minBound().x(), box.minBound().y(),
		               box.minBound().z() + size2.z()),
		    box.maxBound());
	}
	return {left, right};
}
}  // namespace util