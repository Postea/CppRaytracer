#pragma once
#include "Vec3.h"
#include "Material.h"
#include "Ray.h"

namespace cam {
	class Hit
	{
	public:
		Hit (const util::Vec3& hit, const util::Vec3& n, float t, const  std::shared_ptr<material::Material>& material);
		Ray scattered_ray (const Ray& inc_ray) const;
		util::Vec3 albedo (); //TODO TexelPos
		util::Vec3 emission (); //TODO TexelPos
		bool scatter () const;
		//Attributes
		//TODO TexelPos
		const util::Vec3 hit_, n_;
		const float t_;
		const std::shared_ptr<material::Material> material_;
	};

}

