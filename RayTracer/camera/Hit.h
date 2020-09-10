#pragma once

#include "../material/Material.h"
#include "Ray.h"

namespace cam {
class Hit {
   public:
	Hit(const util::Vec3& hit, const util::Vec3& n, float t,
	    const std::shared_ptr<material::Material>& material);

	Ray scattered_ray(const Ray& inc_ray) const;
	util::Vec3 albedo() const;    // TODO TexelPos
	util::Vec3 emission() const;  // TODO TexelPos
	bool scatter() const;
	util::Vec3 hitpoint() const;
	util::Vec3 normal() const;
	float scalar() const;
	// Attributes
	// TODO TexelPos
   private:
	util::Vec3 hit, n;
	float t;
	std::shared_ptr<material::Material> material;
};

}  // namespace cam
