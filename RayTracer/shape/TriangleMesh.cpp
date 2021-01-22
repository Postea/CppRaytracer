#pragma once

#include "TriangleMesh.h"

#include <cassert>
#include <sstream>
#include <string>

#include "../tools/ObjectLoader.h"

namespace shapes {
// Only a test constructor. Can not be used for actual rendering
TriangleMesh::TriangleMesh(std::vector<Triangle> triangles)
    : triangles(triangles),
      material(nullptr),
      hierarchy(Group(util::identity(), false)) {
}
TriangleMesh::TriangleMesh(std::istream& in,
                           const std::shared_ptr<material::Material>& mat)
    : material(mat), hierarchy(Group(util::identity(), false)) {
	triangles = util::loadObj(in, material);
	hierarchy.setBounds(initBB());
	std::vector<std::shared_ptr<Shape>> v;
	for (auto tri : triangles) v.push_back(std::make_shared<Triangle>(tri));
	hierarch(hierarchy, v, 100);
}
std::optional<cam::Hit> TriangleMesh::intersect(const cam::Ray& r) const {
	if (hierarchy.bounds().intersects(r)) {
		// std::cout << "In Intersect" << std::endl;
		auto hit = hierarchy.intersect(r);
		return hit;
	} else
		return std::nullopt;
}
util::AxisAlignedBoundingBox TriangleMesh::bounds() const {
	return hierarchy.bounds();
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
	for (auto tri : triangles) init = init + tri.bounds();
	return init;
}

void hierarch(Group& group, std::vector<std::shared_ptr<Shape>>& v,
              size_t depth) {
	for (auto tri_ptr : v) assert(group.bounds().contains(tri_ptr->bounds()));

	if (depth == 0) return;

	auto bb_pair = util::splitAABB(group.bounds());
	Group left(util::identity(), false);
	left.setBounds(bb_pair[0]);
	Group right(util::identity(), false);
	right.setBounds(bb_pair[1]);
	std::vector<std::shared_ptr<Shape>> left_non_leaves;
	std::vector<std::shared_ptr<Shape>> right_non_leaves;
	for (auto tri_ptr : v) {
		if (left.bounds().contains(tri_ptr->bounds())) {
			left.add(tri_ptr);
			left_non_leaves.push_back(tri_ptr);
		} else if (right.bounds().contains(tri_ptr->bounds())) {
			right.add(tri_ptr);
			right_non_leaves.push_back(tri_ptr);
		} else {
			assert(left.bounds().partiallyContains(tri_ptr->bounds()));
			assert(right.bounds().partiallyContains(tri_ptr->bounds()));
			group.add(tri_ptr);
		}
	}
	v.clear();
	std::cout << depth << " Left: " << left.shapeList.size() << std::endl;
	std::cout << depth << " Right: " << right.shapeList.size() << std::endl;
	std::cout << depth << " Middle: " << group.shapeList.size() << std::endl;
	size_t hierarch_min_cluster_size = 1;
	if (left.shapeList.size() >= hierarch_min_cluster_size) {
		group.add(left);
		hierarch(left, left_non_leaves, depth - 1);
	}
	if (right.shapeList.size() >= hierarch_min_cluster_size) {
		group.add(right);
		hierarch(right, right_non_leaves, depth - 1);
	}
}
}  // namespace shapes
