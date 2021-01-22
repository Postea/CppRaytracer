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
class TriangleMesh : public Light, public Shape {
   public:
	TriangleMesh(std::vector<Triangle> triangles);
	TriangleMesh(std::istream& in,
	             const std::shared_ptr<material::Material>& mat);
	std::optional<cam::Hit> intersect(const cam::Ray& r) const override;
	util::AxisAlignedBoundingBox bounds() const override;

	util::SurfacePoint sampleLight() const override;
	util::Vec3 calculateLightEmission(const util::SurfacePoint& p,
	                                  const util::Vec3& d) const override;

   public:
	std::shared_ptr<material::Material> material;
	std::vector<Triangle> triangles;
	std::vector<Triangle> leaves;
	std::vector<TriMeshNode> hierarchy;

   private:
	util::AxisAlignedBoundingBox initBB();
	std::optional<cam::Hit> TriangleMesh::intersect(size_t i,
	                                                const cam::Ray& r) const;
	void hierarch(size_t i, const std::vector<std::shared_ptr<Triangle>> v);
};
}  // namespace shapes
