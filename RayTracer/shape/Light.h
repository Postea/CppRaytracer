#pragma once
#include "../tools/SurfacePoint.h"

namespace shapes {
class Light {
   public:
	virtual util::SurfacePoint sampleLight() const = 0;
};
}  // namespace shapes