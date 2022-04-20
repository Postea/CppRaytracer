#pragma once
#include "../camera/Hit.h"

namespace shapes {
class Light {
   public:
	virtual util::SurfacePoint sample_light(const cam::Hit& h) const = 0;

	virtual util::Vec3 light_emission(const util::SurfacePoint& p) const = 0;

	virtual float light_pdf(const util::SurfacePoint& p,
	                        const util::Vec3& dl_out) const = 0;
};
}  // namespace shapes