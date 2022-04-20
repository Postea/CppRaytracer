#pragma once
#include "Mat4.h"

namespace util {
class Transformation {
   public:
	Transformation(const Mat4& matrix);
	const Mat4 to_world, from_world, to_world_n, from_world_n;
};
}  // namespace util