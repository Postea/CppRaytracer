#pragma once

#include "../../sampling/Sampler.h"

namespace material {
class Checkerboard : public util::Sampler {
   public:
	Checkerboard(int size);
	util::Vec3 color(float x, float y) const override;

   private:
	float size;
};
}  // namespace material
