#pragma once

#include "../camera/Hit.h"
#include "../camera/Ray.h"

namespace shapes {
class Shape {
   public:
	virtual std::shared_ptr<cam::Hit> intersect(const cam::Ray& r) const = 0;
};
}  // namespace shapes