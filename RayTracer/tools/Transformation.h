#pragma once
#include "Mat4.h"

namespace util {
class Transformation {
   public:
	Transformation(Mat4 matrix);
	const Mat4 toWorld, fromWorld, toWorldN;
};
}  // namespace util