#pragma once

#include "../../sampling/Sampler.h"

namespace material {
class Checkerboard : public util::Sampler {
   public:
	Checkerboard(int size, util::Vec3 c1 = util::Vec3(0),
	             util::Vec3 c2 = util::Vec3(1));
	util::Vec3 color(float x, float y) const override;

   private:
	float size;
	util::Vec3 c1, c2;
};
}  // namespace material
