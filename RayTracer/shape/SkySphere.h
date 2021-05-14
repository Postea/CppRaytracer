#pragma once

#include "../sampling/Image.h"
#include "../sampling/Sampler.h"
#include "Shape.h"
#include "Sphere.h"

namespace shapes {
class SkySphere : public Sphere {
   public:
	SkySphere(const std::shared_ptr<util::Sampler>& sampler);
	SkySphere(const std::shared_ptr<util::Sampler>& sampler,
	           util::Image& distribution);
	float lightPdf(const util::SurfacePoint& p,
	               const util::Vec3& dl_out) const override;
	// We should have to flip the normal, but it does not matter
	// std::optional<cam::Hit> intersect(const cam::Ray& r) const override;
	util::AxisAlignedBoundingBox bounds() const override;
};
}  // namespace shapes
