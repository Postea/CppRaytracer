#include "Ray.h"
#include "Vec3.h"


namespace cam {
	Ray::Ray (const util::Vec3& x0, const util::Vec3& d, float tmin, float tmax, bool normalize) :x0_ (x0), tmin_ (tmin), tmax_ (tmax), normalize_ (normalize)
	{
		if (normalize_)
			d_ = d.normalize ();
		else
			d_ = d;
	}
	util::Vec3 Ray::operator()(float t) const {
		return x0_ + (d_ * t);
	}
	bool Ray::in_range(float t) const {
		return t <= tmax_ && t >= tmin_;
	}
	std::ostream& operator<<(std::ostream& os, const cam::Ray& rhs)
	{
		os << "(" << rhs.x0_ << " " << rhs.d_ << ")";
		return os;
	}
}
