#pragma once

#include "Light.h"
#include "Shape.h"

namespace shapes {
class Rectangle : public Light, public Shape {
   public:
	Rectangle(float width, float depth, bool two_faced,
	               const std::shared_ptr<material::Material>& material);

	std::optional<cam::Hit> intersect(const cam::Ray& r) const override;
	util::AxisAlignedBoundingBox bounds() const override;

	std::pair<float, float> texture_coordinates(const util::Vec3& pos) const;
	util::Vec3 texture_coordinates(const std::pair<float, float>& texel) const;

	util::SurfacePoint sample_light(const cam::Hit& h) const override;
	util::Vec3 light_emission(const util::SurfacePoint& p) const override;
	float light_pdf(const util::SurfacePoint& p,
	                const util::Vec3& dl_out) const override;

   private:
	std::shared_ptr<material::Material> material;
	const float width, depth;
	const bool two_faced;
};
}  // namespace shapes
