#pragma once

#include "Light.h"
#include "Shape.h"

namespace shapes {
class Sphere : public Light, public Shape {
   public:
	Sphere(float radius, const std::shared_ptr<material::Material>& material);
	std::optional<cam::Hit> intersect(const cam::Ray& r) const override;
	std::pair<float, float> texture_coordinates(
	    const util::Vec3& pos) const override;
	util::AxisAlignedBoundingBox bounds() const override;
	util::SurfacePoint sampleLight(const cam::Hit& h) const override;
	util::Vec3 calculateLightEmission(const util::SurfacePoint& p,
	                                  const util::Vec3& d) const override;

   private:
	std::shared_ptr<material::Material> material;
	const float radius;
};
}  // namespace shapes
