#pragma once

#include <memory>
#include <optional>

#include "../camera/Hit.h"
#include "../tools/AxisAlignedBoundingBox.h"
#include "../tools/Vertex.h"
#include "Light.h"
#include "Shape.h"

namespace shapes {
class Triangle : public Light, public Shape {
   public:
	Triangle(util::Vertex p1, util::Vertex p2, util::Vertex p3,
	         const std::shared_ptr<material::Material>& material);
	std::optional<cam::Hit> intersect(const cam::Ray& r) const;
	std::pair<float, float> texture_coordinates(const util::Vec3& pos) const;
	util::AxisAlignedBoundingBox bounds() const;
	void recalculateBB();

	util::SurfacePoint sampleLight(const cam::Hit& h) const;
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
