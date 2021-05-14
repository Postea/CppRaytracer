#pragma once

#include "Light.h"
#include "Shape.h"

namespace shapes {
class CirclePlane : public Light, public Shape {
   public:
	CirclePlane(float radius, bool twofaced,
	            const std::shared_ptr<material::Material>& material);
	std::optional<cam::Hit> intersect(const cam::Ray& r) const override;
	std::pair<float, float> texture_coordinates(
	    const util::Vec3& pos) const override;
	util::AxisAlignedBoundingBox bounds() const override;

	util::SurfacePoint sampleLight(const cam::Hit& h) const override;
	util::Vec3 lightEmission(const util::SurfacePoint& p) const override;
	float lightPdf(const util::SurfacePoint& p,
	               const util::Vec3& dl_out) const override;

   private:
	std::shared_ptr<material::Material> material;
	const float radius;
	const bool twofaced;
};
}  // namespace shapes
