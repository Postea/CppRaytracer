#pragma once

#include "TriangleMesh.h"

#include <cassert>
#include <limits>
#include <sstream>
#include <string>

#include "../tools/ObjectLoader.h"

namespace shapes {
// Only a test constructor. Can not be used for actual rendering
TriangleMesh::TriangleMesh(std::vector<Triangle> triangles)
    : material(nullptr), hierarchy({}) {
}
TriangleMesh::TriangleMesh(std::istream& in,
                           const std::shared_ptr<material::Material>& mat)
    : material(mat), hierarchy({}) {
	auto triangles = util::load_obj(in, material);
	hierarchy.push_back({init_bb(triangles), -1, -1, -1, -1});
	std::vector<std::shared_ptr<Triangle>> v;
	for (auto tri : triangles) v.push_back(std::make_shared<Triangle>(tri));
	hierarch(0, v);
	triangles.clear();
}

std::optional<cam::Hit> TriangleMesh::intersect(const cam::Ray& r) const {
	return intersect(0, r);
}
std::optional<cam::Hit> TriangleMesh::intersect(size_t i,
                                                const cam::Ray& r) const {
	if (!hierarchy[i].bb.intersects(r)) return std::nullopt;
	std::array<cam::Hit, 3> hits = {
	    cam::Hit(util::Vec3({}), util::Vec3({}), {},
	             std::numeric_limits<float>::infinity(), nullptr),
	    cam::Hit(util::Vec3({}), util::Vec3({}), {},
	             std::numeric_limits<float>::infinity(), nullptr),
	    cam::Hit(util::Vec3({}), util::Vec3({}), {},
	             std::numeric_limits<float>::infinity(), nullptr)};
	// Check left
	size_t left_i = hierarchy[i].left;
	std::optional<cam::Hit> left_hit = std::nullopt;
	if (left_i != -1) left_hit = intersect(left_i, r);
	hits[0] = left_hit.value_or(cam::Hit(util::Vec3({}), util::Vec3({}), {},
	                                     std::numeric_limits<float>::infinity(),
	                                     nullptr));
	// Check right
	size_t right_i = hierarchy[i].right;
	std::optional<cam::Hit> right_hit = std::nullopt;
	if (right_i != -1) right_hit = intersect(right_i, r);
	hits[1] = right_hit.value_or(
	    cam::Hit(util::Vec3({}), util::Vec3({}), {},
	             std::numeric_limits<float>::infinity(), nullptr));

	std::optional<cam::Hit> temp = std::nullopt;
	std::optional<cam::Hit> mid_hit = std::nullopt;
	int_fast16_t bound = hierarchy[i].leaves_i + hierarchy[i].leaves_size - 1;

	for (size_t tri_i = hierarchy[i].leaves_i; tri_i <= bound; tri_i++) {
		auto tri = leaves[tri_i];
		std::optional<cam::Hit> temp = tri.intersect(r);
		if (temp) {
			if (r.in_range(temp->scalar())) {
				if (!mid_hit) {
					mid_hit = temp;
				} else if (mid_hit->scalar() > temp->scalar()) {
					mid_hit = temp;
				}
			}
		}
	}
	hits[2] = mid_hit.value_or(cam::Hit(util::Vec3({}), util::Vec3({}), {},
	                                    std::numeric_limits<float>::infinity(),
	                                    nullptr));
	std::sort(hits.begin(), hits.end(),
	          [](cam::Hit a, cam::Hit b) { return a.scalar() < b.scalar(); });
	if (hits[0].material == nullptr) return std::nullopt;
	return hits[0];
}

util::AxisAlignedBoundingBox TriangleMesh::bounds() const {
	return hierarchy[0].bb;
}

util::AxisAlignedBoundingBox TriangleMesh::init_bb(
    const std::vector<Triangle>& triangles) {
	util::AxisAlignedBoundingBox init = triangles[0].bounds();
	for (const auto& tri : triangles) init = init + tri.bounds();
	return init;
}

void TriangleMesh::hierarch(size_t i,
                            const std::vector<std::shared_ptr<Triangle>>& v) {
	std::array<util::AxisAlignedBoundingBox, 2> bb_pair =
	    util::split_bb(hierarchy[i].bb);
	TriMeshNode left({bb_pair[0], -1, -1, -1, -1});
	TriMeshNode right({bb_pair[1], -1, -1, -1, -1});
	std::vector<std::shared_ptr<Triangle>> left_non_leaves;
	std::vector<std::shared_ptr<Triangle>> right_non_leaves;
	std::vector<std::shared_ptr<Triangle>> middle;
	for (auto tri_ptr : v) {
		if (left.bb.contains(tri_ptr->bounds()))
			left_non_leaves.push_back(tri_ptr);
		else if (right.bb.contains(tri_ptr->bounds()))
			right_non_leaves.push_back(tri_ptr);
		else
			middle.push_back(tri_ptr);
	}

	// Handle middle leaves
	if (!middle.empty()) {
		hierarchy[i].leaves_i = leaves.size();
		hierarchy[i].leaves_size = middle.size();
		for (auto tri_ptr : middle) leaves.push_back(*tri_ptr);
	}
	// Handle left box
	if (!left_non_leaves.empty()) {
		hierarchy[i].left = hierarchy.size();
		hierarchy.push_back(left);
	}
	// Handle right box
	if (!right_non_leaves.empty()) {
		hierarchy[i].right = hierarchy.size();
		hierarchy.push_back(right);
	}

	// Handle recursion
	if (!left_non_leaves.empty()) hierarch(hierarchy[i].left, left_non_leaves);
	if (!right_non_leaves.empty())
		hierarch(hierarchy[i].right, right_non_leaves);
}
}  // namespace shapes
