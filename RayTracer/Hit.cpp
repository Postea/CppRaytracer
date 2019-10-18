#include "Hit.h"
#include <limits>


namespace cam {
	Hit::Hit (const util::Vec3& hit, const util::Vec3& n, float t, const std::shared_ptr<material::Material>& material)
		:hit_ (hit), n_ (n), t_ (t), material_ (material) {}
	bool Hit::scatter () const
	{
		return material_->scatter();
	}
	Ray Hit::scattered_ray (const Ray& inc_ray) const
	{
		return Ray (hit_, material_->scattered_d(inc_ray.d_, n_), std::numeric_limits<float>::epsilon(), inc_ray.tmax_, true);
	}
	util::Vec3 Hit::albedo ()
	{
		return material_->albedo (0,0);
	}
	util::Vec3 Hit::emission ()
	{
		return material_->emission (0,0);
	}
}