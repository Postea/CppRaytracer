#include "Ray.h"

namespace cam {
Ray::Ray(const util::Vec3& x0, const util::Vec3& arg_d, float tmin, float tmax, bool normalize)
    : x0(x0)
    , tmin(tmin)
    , tmax(tmax)
    , normalize(normalize)
{
    if (normalize)
        d = arg_d.normalize();
    else
        d = arg_d;
}
util::Vec3 Ray::operator()(float t) const
{
    return x0 + (d * t);
}
bool Ray::in_range(float t) const
{
    return t <= tmax && t >= tmin;
}
std::ostream& operator<<(std::ostream& os, const cam::Ray& rhs)
{
    os << "(" << rhs.x0 << " " << rhs.d << ")";
    return os;
}
}
