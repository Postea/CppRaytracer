#pragma once

#include "../../sampling/Sampler.h"

namespace material {
class Constant : public util::Sampler {
   public:
	Constant(const util::Vec3& albedo);

	util::Vec3 color(float x, float y) const override;

   private:
	const util::Vec3 albedo;
};
}  // namespace material
