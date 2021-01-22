#pragma once

#include <vector>

#include "../tools/Vertex.h"
#include "Group.h"
#include "Light.h"
#include "Shape.h"
#include "Triangle.h"

namespace shapes {
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
	Group hierarchy;

   private:
	util::AxisAlignedBoundingBox initBB();
};
void hierarch(Group& group, std::vector<std::shared_ptr<Shape>>& v,
              size_t depth = 5);
}  // namespace shapes