#pragma once
#include "../camera/Hit.h"

namespace shapes {
class Light {
   public:
	virtual util::SurfacePoint sampleLight(const cam::Hit& h) const = 0;
	virtual util::Vec3 lightEmission(const util::SurfacePoint& p) const = 0;
	virtual float lightPdf(const util::SurfacePoint& p,
	                       const util::Vec3& dl_out) const = 0;

	virtual std::optional<cam::Hit> intersect(const cam::Ray& r) const = 0;
};
}  // namespace shapes