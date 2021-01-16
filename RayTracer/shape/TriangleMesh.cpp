#pragma once

#include "TriangleMesh.h"

#include <sstream>
#include <string>

#include "../tools/ObjectLoader.h"

namespace shapes {
TriangleMesh::TriangleMesh(std::vector<Triangle> triangles)
    : triangles(triangles),
      material(nullptr),
      hierarchy(Group(util::identity())) {
}
TriangleMesh::TriangleMesh(std::istream& in,
                           const std::shared_ptr<material::Material>& material)
    : material(material), hierarchy(Group(util::identity())) {
	triangles = util::loadObj(in, material);
}
std::optional<cam::Hit> TriangleMesh::intersect(const cam::Ray& r) const {
	std::optional<cam::Hit> result = std::nullopt;

	for (auto tri : triangles) {
		// if (tri.bounds().intersects(r)) {
		std::optional<cam::Hit> temp = tri.intersect(r);
		if (temp) {
			if (r.in_range(temp->scalar())) {
				if (!result) {
					result = temp;
				} else if (result->scalar() > temp->scalar()) {
					result = temp;
				}
			}
		}
		//}
	}
	return result;
}
util::AxisAlignedBoundingBox TriangleMesh::bounds() const {
	return util::AxisAlignedBoundingBox(util::Vec3(-2), util::Vec3(2));
}

util::SurfacePoint TriangleMesh::sampleLight() const {
	return util::SurfacePoint(util::Vec3(), 0, material);
}
util::Vec3 TriangleMesh::calculateLightEmission(const util::SurfacePoint& p,
                                                const util::Vec3& d) const {
	return util::Vec3();
}
util::AxisAlignedBoundingBox TriangleMesh::initBB() {
	util::AxisAlignedBoundingBox init = triangles[0].bounds();
	for (auto tri : triangles) init + tri.bounds();
	return init;
}
}  // namespace shapes
