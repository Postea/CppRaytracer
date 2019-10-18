#pragma once

#include "Vec3.h"
namespace util {
	class Sampler
	{
	public:
		virtual Vec3 color (float x, float y) const = 0;
	};
}

