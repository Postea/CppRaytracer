#pragma once

#include "../../sampling/Sampler.h"
#include "../../tools/Vec3.h"

namespace material {
class Constant : public util::Sampler {
   private:
	const util::Vec3 albedo;

   public:
	Constant(const util::Vec3& albedo);

	util::Vec3 color(float x, float y) const override;
};
}  // namespace material
