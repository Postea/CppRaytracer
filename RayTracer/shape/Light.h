#pragma once
#include "../camera/Ray.h"

namespace shapes {
class Light {
   public:
	virtual cam::Ray shadowRay() const = 0;
};
}  // namespace shapes