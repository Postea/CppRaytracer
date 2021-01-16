#pragma once

#include "Vec3.h"

namespace util {
class Vertex {
   public:
	util::Vec3 position;
	util::Vec3 texcoord;
	util::Vec3 normal;
};
}  // namespace util