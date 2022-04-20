#pragma once

#include <vector>

#include "../tools/Vertex.h"
#include "Group.h"
#include "Light.h"
#include "Shape.h"
#include "Triangle.h"

namespace shapes {

struct TriMeshNode {
	util::AxisAlignedBoundingBox bb;
	int_fast16_t left;
	int_fast16_t right;
	int_fast16_t leaves_i;
	int_fast16_t leaves_size;
};
class TriangleMesh : public Shape {
	// TriangleMesh does not support uv coordinates
   public:
	TriangleMesh(std::vector<Triangle> triangles);
	TriangleMesh(std::istream& in,
	             const std::shared_ptr<material::Material>& mat);

	std::optional<cam::Hit> intersect(const cam::Ray& r) const override;
	util::AxisAlignedBoundingBox bounds() const override;

   public:
	std::shared_ptr<material::Material> material;
	std::vector<Triangle> leaves;
	std::vector<TriMeshNode> hierarchy;

   private:
	util::AxisAlignedBoundingBox init_bb(
	    const std::vector<Triangle>& triangles);
	std::optional<cam::Hit> TriangleMesh::intersect(size_t i,
	                                                const cam::Ray& r) const;
	void hierarch(size_t i, const std::vector<std::shared_ptr<Triangle>>& v);
};
}  // namespace shapes
