#pragma once

#include <random>

#include "../sampling/Sampler.h"
#include "../tools/Vec3.h"

namespace material {

class Material {
   public:
	virtual util::Vec3 albedo(float texel_x, float texel_y) const = 0;
	virtual util::Vec3 emission(float texel_x, float texel_y) const = 0;
	virtual util::Vec3 scattered_d(const util::Vec3& d,
	                               const util::Vec3& n) const = 0;
	virtual bool scatter() const = 0;
};

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_real_distribution<> dis(-1.0, 1.0);
util::Vec3 rand_vec3();
bool in_circle(const util::Vec3& v);
util::Vec3 rand_vec3_in_circle(float r);
}  // namespace material
