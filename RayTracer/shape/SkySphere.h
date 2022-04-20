#pragma once

#include "../sampling/Image.h"
#include "../sampling/Sampler.h"
#include "Shape.h"
#include "Sphere.h"

namespace shapes {
class SkySphere : public Sphere, public Light {
   public:
	SkySphere(const std::shared_ptr<util::Sampler>& sampler);
	SkySphere(const std::shared_ptr<util::Sampler>& sampler,
	          util::Image& distribution, float intensity = 1);

	util::AxisAlignedBoundingBox bounds() const override;

	util::SurfacePoint sample_light(const cam::Hit& h) const override;
	util::Vec3 light_emission(const util::SurfacePoint& p) const override;
	virtual float light_pdf(const util::SurfacePoint& p,
	                        const util::Vec3& dl_out) const override;
};
}  // namespace shapes
