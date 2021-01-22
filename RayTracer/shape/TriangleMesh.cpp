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
	return intersect(0, r);
}
std::optional<cam::Hit> TriangleMesh::intersect(size_t i,
                                                const cam::Ray& r) const {
	std::array<cam::Hit, 3> hits = {
	    cam::Hit(util::Vec3(0), util::Vec3(0),
	             std::numeric_limits<float>::infinity(), nullptr),
	    cam::Hit(util::Vec3(0), util::Vec3(0),
	             std::numeric_limits<float>::infinity(), nullptr),
	    cam::Hit(util::Vec3(0), util::Vec3(0),
	             std::numeric_limits<float>::infinity(), nullptr)};
	// Check left
	size_t left_i = hierarchy[i].left;
	std::optional<cam::Hit> left_hit = std::nullopt;
	if (left_i != -1) left_hit = intersect(left_i, r);
	hits[0] = left_hit.value_or(cam::Hit(util::Vec3(0), util::Vec3(0),
	                                     std::numeric_limits<float>::infinity(),
	                                     nullptr));
	// Check right
	size_t right_i = hierarchy[i].right;
	std::optional<cam::Hit> right_hit = std::nullopt;
	if (right_i != -1) right_hit = intersect(right_i, r);
	hits[1] = right_hit.value_or(
	    cam::Hit(util::Vec3(0), util::Vec3(0),
	             std::numeric_limits<float>::infinity(), nullptr));

	std::optional<cam::Hit> temp = std::nullopt;
	std::optional<cam::Hit> mid_hit = std::nullopt;
	int_fast16_t bound = hierarchy[i].leaves_i + hierarchy[i].leaves_size - 1;
	assert(!(hierarchy[i].leaves_i == -1 ^ hierarchy[i].leaves_size == -1));
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
	hits[2] = mid_hit.value_or(cam::Hit(util::Vec3(0), util::Vec3(0),
	                                    std::numeric_limits<float>::infinity(),
	                                    nullptr));
	std::sort(hits.begin(), hits.end(),
	          [](cam::Hit a, cam::Hit b) { return a.scalar() < b.scalar(); });
	if (hits[0].material == nullptr) return std::nullopt;
	// for (auto hit : hits) std::cout << hit << std::endl;
	return hits[0];
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
			if (!left.bb.partiallyContains(tri_ptr->bounds())) {
				std::cout << "Node " << hierarchy[i].bb << std::endl;
				std::cout << left.bb << "//////" << tri_ptr->bounds()
				          << std::endl;
			}
			assert(left.bb.partiallyContains(tri_ptr->bounds()));
			assert(right.bb.partiallyContains(tri_ptr->bounds()));
			middle.push_back(tri_ptr);
		}
	}
	// std::cout << depth << " Left: " << left.shapeList.size() <<
	// std::endl; std::cout << depth << " Right: " << right.shapeList.size()
	// << std::endl; std::cout << depth << " Middle: " <<
	// group.shapeList.size() << std::endl;

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
