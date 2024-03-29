#pragma once

#include "../material/Material.h"
#include "../tools/SurfacePoint.h"
#include "Ray.h"

namespace cam {
class Hit : public util::SurfacePoint {
   public:
	Hit(const util::Vec3& hit, const util::Vec3& n,
	    const std::pair<float, float>& uv, float t,
	    const std::shared_ptr<material::Material>& material);

	float scalar() const;
	friend std::ostream& operator<<(std::ostream& os, const cam::Hit& rhs);

   private:
	float t;
};

}  // namespace cam
