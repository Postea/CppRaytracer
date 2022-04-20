#pragma once

#include <memory>
#include <optional>

#include "../camera/Hit.h"
#include "../tools/AxisAlignedBoundingBox.h"
#include "../tools/Vertex.h"
#include "Light.h"
#include "Shape.h"

namespace shapes {
class Triangle : public Shape {
	// Triangles do not support uv coordinates
   public:
	Triangle(const util::Vertex& p1, const util::Vertex& p2,
	         const util::Vertex& p3,
	         const std::shared_ptr<material::Material>& material);
	std::optional<cam::Hit> intersect(const cam::Ray& r) const;
	util::AxisAlignedBoundingBox bounds() const;

	void recalculate_bb();

   private:
	std::shared_ptr<material::Material> material;
	const util::Vertex p1;
	const util::Vertex p2;
	const util::Vertex p3;
	util::AxisAlignedBoundingBox bb;
};
}  // namespace shapes
