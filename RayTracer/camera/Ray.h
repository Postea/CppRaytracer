#pragma once
#include "../tools/Vec3.h"

namespace cam {
class Ray {
   public:
	Ray(const util::Vec3& x0, const util::Vec3& d, float tmin, float tmax,
	    bool normalize);

	util::Vec3 operator()(float t) const;
	bool in_range(float t) const;
	friend std::ostream& operator<<(std::ostream& os, const cam::Ray& rhs);

	const util::Vec3 x0;
	util::Vec3 d;
	const float tmin, tmax;
	const bool normalize;
};
const float epsilon = pow(10, -4);
}  // namespace cam
