#pragma once
#include "../camera/Hit.h"

namespace shapes {
class Light {
   public:
	virtual util::SurfacePoint sampleLight(const cam::Hit& h) const = 0;
	virtual util::Vec3 calculateLightEmission(const util::SurfacePoint& p,
	                                          const util::Vec3& d) const = 0;
};
}  // namespace shapes