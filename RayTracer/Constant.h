#pragma once

#include "Vec3.h"
#include "Sampler.h"

namespace material {
	class Constant : public util::Sampler
	{
	private:
		const util::Vec3 color_;
	public:
		Constant (const util::Vec3& color);

		util::Vec3 color (float x, float y) const override;
	};
}

