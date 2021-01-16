#pragma once

#include <memory>
#include <optional>

#include "../camera/Hit.h"
#include "../tools/AxisAlignedBoundingBox.h"
#include "../tools/Vertex.h"

namespace shapes {
class Triangle {
   public:
	Triangle(util::Vertex p1, util::Vertex p2, util::Vertex p3,
	         const std::shared_ptr<material::Material>& material);
	std::optional<cam::Hit> intersect(const cam::Ray& r) const;
	util::AxisAlignedBoundingBox bounds() const;
	void recalculateBB();

	util::SurfacePoint sampleLight() const;
	util::Vec3 calculateLightEmission(const util::SurfacePoint& p,
	                                  const util::Vec3& d) const;

	const std::shared_ptr<material::Material>& material;

   private:
	const util::Vertex p1;
	const util::Vertex p2;
	const util::Vertex p3;
	util::AxisAlignedBoundingBox bb;
};
}  // namespace shapes
