#pragma once

#include "../tools/Mat4.h"
#include "../tools/Transformation.h"
#include "Light.h"

namespace shapes {
class LightSingleGroup : public Light {
   public:
	LightSingleGroup(const util::Transformation& trans,
	                 std::shared_ptr<Light> shape);
	LightSingleGroup(const util::Mat4& matrix, std::shared_ptr<Light> shape);

	util::SurfacePoint sampleLight(const cam::Hit& h) const override;
	util::Vec3 lightEmission(const util::SurfacePoint& p) const override;
	float lightPdf(const util::SurfacePoint& p,
	               const util::Vec3& dl_out) const override;

   private:
	std::shared_ptr<Light> light;
	util::Transformation transform;
};
}  // namespace shapes
