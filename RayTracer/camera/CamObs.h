#pragma once

#include "../tools/Mat4.h"
#include "Ray.h"

namespace cam {
class CamObs {
   public:
	CamObs(const util::Mat4& mat, float theta, int width, int height);

	Ray create(float x, float y) const;

	const int width, height;

   private:
	const util::Mat4 mat;
	const float theta;
};
}  // namespace cam
