#include "Constant.h"

namespace material {
	Constant::Constant (const util::Vec3& color): color_(color)
	{
	}
	util::Vec3 Constant::color (float x, float y) const {
		return color_;
	}
}
