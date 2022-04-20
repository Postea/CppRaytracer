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
	Vec3 v1 = rhs.transform_point(Vec3(min.x(), min.y(), min.z()));
	Vec3 v2 = rhs.transform_point(Vec3(min.x(), min.y(), max.z()));
	Vec3 v3 = rhs.transform_point(Vec3(min.x(), max.y(), min.z()));
	Vec3 v4 = rhs.transform_point(Vec3(min.x(), min.y(), max.z()));
	Vec3 v5 = rhs.transform_point(Vec3(max.x(), min.y(), min.z()));
	Vec3 v6 = rhs.transform_point(Vec3(min.x(), min.y(), max.z()));
	Vec3 v7 = rhs.transform_point(Vec3(min.x(), max.y(), min.z()));
	Vec3 v8 = rhs.transform_point(Vec3(min.x(), max.y(), max.z()));
	float min_x = std::min<float>(
	    {v1.x(), v2.x(), v3.x(), v4.x(), v5.x(), v6.x(), v7.x(), v8.x()});
	float min_y = std::min<float>(
	    {v1.y(), v2.y(), v3.y(), v4.y(), v5.y(), v6.y(), v7.y(), v8.y()});
	float min_z = std::min<float>(
	    {v1.z(), v2.z(), v3.z(), v4.z(), v5.z(), v6.z(), v7.z(), v8.z()});
	float max_x = std::max<float>(
	    {v1.x(), v2.x(), v3.x(), v4.x(), v5.x(), v6.x(), v7.x(), v8.x()});
	float max_y = std::max<float>(
	    {v1.y(), v2.y(), v3.y(), v4.y(), v5.y(), v6.y(), v7.y(), v8.y()});
	float max_z = std::max<float>(
	    {v1.z(), v2.z(), v3.z(), v4.z(), v5.z(), v6.z(), v7.z(), v8.z()});

	return AxisAlignedBoundingBox(Vec3(min_x, min_y, min_z),
	                              Vec3(max_x, max_y, max_z));
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

bool AxisAlignedBoundingBox::contains(const AxisAlignedBoundingBox& bb) const {
	std::array<Vec3, 8> vertices = {
	    Vec3(bb.min_bound().x(), bb.min_bound().y(), bb.min_bound().z()),
	    Vec3(bb.min_bound().x(), bb.min_bound().y(), bb.max_bound().z()),
	    Vec3(bb.min_bound().x(), bb.max_bound().y(), bb.min_bound().z()),
	    Vec3(bb.min_bound().x(), bb.max_bound().y(), bb.max_bound().z()),
	    Vec3(bb.max_bound().x(), bb.min_bound().y(), bb.min_bound().z()),
	    Vec3(bb.max_bound().x(), bb.min_bound().y(), bb.max_bound().z()),
	    Vec3(bb.max_bound().x(), bb.max_bound().y(), bb.min_bound().z()),
	    Vec3(bb.max_bound().x(), bb.max_bound().y(), bb.max_bound().z())};
	for (Vec3 v : vertices)
		if (!contains(v)) return false;

	return true;
}
// This Method is not entirely correct. It only checks if the corners of this bb
// is inside the arguments bb. This should be enough for the splitting algorithm
// to create a hierarchy. Theoretically there are overlapping bbs, where no
// corner is inside the others.

bool AxisAlignedBoundingBox::partially_contains(
    const AxisAlignedBoundingBox& bb) const {
	std::array<Vec3, 8> vertices = {
	    Vec3(bb.min_bound().x(), bb.min_bound().y(), bb.min_bound().z()),
	    Vec3(bb.min_bound().x(), bb.min_bound().y(), bb.max_bound().z()),
	    Vec3(bb.min_bound().x(), bb.max_bound().y(), bb.min_bound().z()),
	    Vec3(bb.min_bound().x(), bb.max_bound().y(), bb.max_bound().z()),
	    Vec3(bb.max_bound().x(), bb.min_bound().y(), bb.min_bound().z()),
	    Vec3(bb.max_bound().x(), bb.min_bound().y(), bb.max_bound().z()),
	    Vec3(bb.max_bound().x(), bb.max_bound().y(), bb.min_bound().z()),
	    Vec3(bb.max_bound().x(), bb.max_bound().y(), bb.max_bound().z())};
	for (Vec3 v : vertices)
		if (contains(v)) return true;
	return false;
}
float AxisAlignedBoundingBox::size() const {
	return (max_bound() - min_bound()).length();
}
Vec3 AxisAlignedBoundingBox::center() const {
	return (max + min) / 2;
}
Vec3 AxisAlignedBoundingBox::min_bound() const {
	return min;
}
Vec3 AxisAlignedBoundingBox::max_bound() const {
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
// Pair is 0->Left 1->Right
// From cgg Abgaben of Prof. Tramberend
std::array<AxisAlignedBoundingBox, 2> split_bb(
    const AxisAlignedBoundingBox& box) {
	util::Vec3 size2 = (box.max_bound() - box.min_bound()) / 2;
	AxisAlignedBoundingBox left;
	if (size2.x() >= size2.y() && size2.x() >= size2.z()) {
		left = AxisAlignedBoundingBox(
		    box.min_bound(),
		    util::Vec3(box.min_bound().x() + size2.x(), box.max_bound().y(),
		               box.max_bound().z()));
	} else if (size2.y() >= size2.x() && size2.y() >= size2.z()) {
		left = AxisAlignedBoundingBox(
		    box.min_bound(),
		    util::Vec3(box.max_bound().x(), box.min_bound().y() + size2.y(),
		               box.max_bound().z()));
	} else {
		left = AxisAlignedBoundingBox(
		    box.min_bound(),
		    util::Vec3(box.max_bound().x(), box.max_bound().y(),
		               box.min_bound().z() + size2.z()));
	}
	AxisAlignedBoundingBox right;
	if (size2.x() >= size2.y() && size2.x() >= size2.z()) {
		right = AxisAlignedBoundingBox(
		    util::Vec3(box.min_bound().x() + size2.x(), box.min_bound().y(),
		               box.min_bound().z()),
		    box.max_bound());
	} else if (size2.y() >= size2.x() && size2.y() >= size2.z()) {
		right = AxisAlignedBoundingBox(
		    util::Vec3(box.min_bound().x(), box.min_bound().y() + size2.y(),
		               box.min_bound().z()),
		    box.max_bound());
	} else {
		right = AxisAlignedBoundingBox(
		    util::Vec3(box.min_bound().x(), box.min_bound().y(),
		               box.min_bound().z() + size2.z()),
		    box.max_bound());
	}
	return {left, right};
}
}  // namespace util