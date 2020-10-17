#pragma once

#include "../camera/Ray.h"
#include "../material/Material.h"

namespace util {
class SurfacePoint {
   public:
	SurfacePoint(const util::Vec3& point, const util::Vec3& n,
	             const std::shared_ptr<material::Material>& material);

	cam::Ray scattered_ray(const cam::Ray& inc_ray) const;
	util::Vec3 albedo() const;    // TODO TexelPos
	util::Vec3 emission() const;  // TODO TexelPos
	bool scatter(const util::Vec3& d, const util::Vec3& n) const;
	util::Vec3 point() const;
	util::Vec3 normal() const;
	float calculateLightMultiplier(const util::Vec3& d_in,
	                               const util::Vec3& d_out,
	                               const util::Vec3& n) const;
	// Attributes
	// TODO TexelPos
	std::shared_ptr<material::Material> material;

   private:
	util::Vec3 x, n;
};

}  // namespace util
