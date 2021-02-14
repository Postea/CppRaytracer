#pragma once

#include "Light.h"
#include "Shape.h"

namespace shapes {
class RectanglePlane : public Light, public Shape {
   public:
	RectanglePlane(float width, float depth, bool twofaced,
	               const std::shared_ptr<material::Material>& material);
	std::optional<cam::Hit> intersect(const cam::Ray& r) const override;
	std::pair<float, float> texture_coordinates(
	    const util::Vec3& pos) const override;
	util::AxisAlignedBoundingBox bounds() const override;

	util::SurfacePoint sampleLight() const override;
	util::Vec3 calculateLightEmission(const util::SurfacePoint& p,
	                                  const util::Vec3& d) const override;

   private:
	std::shared_ptr<material::Material> material;
	const float width, depth;
	const bool twofaced;
};
}  // namespace shapes
