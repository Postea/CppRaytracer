#pragma once

#include "../tools/Mat4.h"
#include "../tools/Transformation.h"
#include "LightShape.h"

namespace shapes {
class SingleGroup : public Shape, public Light {
   public:
	SingleGroup(const util::Transformation& trans,
	            std::shared_ptr<LightShape> shape);
	SingleGroup(const util::Mat4& matrix, std::shared_ptr<LightShape> shape);

	std::optional<cam::Hit> intersect(const cam::Ray& r) const override;
	util::AxisAlignedBoundingBox bounds() const override;

	util::SurfacePoint sampleLight() const override;

   private:
	util::AxisAlignedBoundingBox boundingVolume;
	std::shared_ptr<LightShape> shape;
	util::Transformation transform;
};
}  // namespace shapes
