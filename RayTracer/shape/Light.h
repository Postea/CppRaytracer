#pragma once
#include "../camera/Hit.h"

namespace shapes {
class Light {
   public:
	virtual cam::Hit sampleLight() const = 0;
};
}  // namespace shapes