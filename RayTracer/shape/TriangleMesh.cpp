#pragma once

#include "TriangleMesh.h"

#include <cassert>
#include <sstream>
#include <string>

#include "../tools/ObjectLoader.h"

namespace shapes {
// Only a test constructor. Can not be used for actual rendering
TriangleMesh::TriangleMesh(std::vector<Triangle> triangles)
    : triangles(triangles), material(nullptr), hierarchy({}) {
}
TriangleMesh::TriangleMesh(std::istream& in,
                           const std::shared_ptr<material::Material>& mat)
    : material(mat), hierarchy({}) {
	triangles = util::loadObj(in, material);
	hierarchy.push_back({initBB(), -1, -1, -1, -1});
	std::vector<std::shared_ptr<Triangle>> v;
	for (auto tri : triangles) v.push_back(std::make_shared<Triangle>(tri));
	hierarch(0, v);
}
std::optional<cam::Hit> TriangleMesh::intersect(const cam::Ray& r) const {
	return std::nullopt;
}
util::AxisAlignedBoundingBox TriangleMesh::bounds() const {
	return hierarchy[0].bb;
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

void TriangleMesh::hierarch(size_t i,
                            std::vector<std::shared_ptr<Triangle>> v) {
	if (v.empty()) return;
	auto bb_pair = util::splitAABB(hierarchy[i].bb);
	TriMeshNode left({bb_pair[0], -1, -1, -1, -1});
	TriMeshNode right({bb_pair[1], -1, -1, -1, -1});
	std::vector<std::shared_ptr<Triangle>> left_non_leaves;
	std::vector<std::shared_ptr<Triangle>> right_non_leaves;
	std::vector<std::shared_ptr<Triangle>> middle;
	for (auto tri_ptr : v) {
		if (left.bb.contains(tri_ptr->bounds())) {
			left_non_leaves.push_back(tri_ptr);
		} else if (right.bb.contains(tri_ptr->bounds())) {
			right_non_leaves.push_back(tri_ptr);
		} else {
			assert(left.bb.partiallyContains(tri_ptr->bounds()));
			assert(right.bb.partiallyContains(tri_ptr->bounds()));
			middle.push_back(tri_ptr);
		}
	}
	// std::cout << depth << " Left: " << left.shapeList.size() << std::endl;
	// std::cout << depth << " Right: " << right.shapeList.size() << std::endl;
	// std::cout << depth << " Middle: " << group.shapeList.size() << std::endl;

	// Handle middle leaves
	hierarchy[i].leaves_i = leaves.size();
	hierarchy[i].leaves_size = middle.size();
	for (auto tri_ptr : middle) leaves.push_back(*tri_ptr);

	// Handle left box
	hierarchy.push_back(left);
	std::cout << i << std::endl;
	hierarchy[i].left = hierarchy.size();
	// Handle right box
	hierarchy.push_back(right);
	hierarchy[i].right = hierarchy.size();

	// Handle recursion
	hierarch(hierarchy.size() - 2, left_non_leaves);
	hierarch(hierarchy.size() - 1, right_non_leaves);

	/*size_t hierarch_min_cluster_size = 1;
	if (left.shapeList.size() >= hierarch_min_cluster_size) {
	    group.add(left);
	    hierarch(left, left_non_leaves, depth - 1);
	}
	if (right.shapeList.size() >= hierarch_min_cluster_size) {
	    group.add(right);
	    hierarch(right, right_non_leaves, depth - 1);
	}*/
}
}  // namespace shapes
