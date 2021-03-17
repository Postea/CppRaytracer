#pragma once

#include "../sampling/Sampler.h"
#include "Shape.h"
#include "Sphere.h"

namespace shapes {
class Background : public Sphere {
   public:
	Background(const std::shared_ptr<util::Sampler>& sampler);
	// We should have to flip the normal, but it does not matter
	// std::optional<cam::Hit> intersect(const cam::Ray& r) const override;
	util::AxisAlignedBoundingBox bounds() const override;
};
}  // namespace shapes
