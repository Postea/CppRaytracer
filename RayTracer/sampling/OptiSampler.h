#pragma once

#include <vector>

#include "../tools/Vec3.h"

namespace util {
class OptiSampler {
   public:
	virtual std::pair<Vec3, std::vector<int64_t>> color_opti(float x,
	                                                         float y) const = 0;
};
}  // namespace util
