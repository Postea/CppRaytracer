#include "Hit.h"
#include <limits>

namespace cam {
Hit::Hit(const util::Vec3& hit, const util::Vec3& n, float t, const std::shared_ptr<material::Material>& material)
    : hit(hit)
    , n(n)
    , t(t)
    , material(material)
{
}
bool Hit::scatter() const
{
    return material->scatter();
}
Ray Hit::scattered_ray(const Ray& inc_ray) const
{
    return Ray(hit, material->scattered_d(inc_ray.d, n), std::numeric_limits<float>::epsilon(), inc_ray.tmax, true);
}
util::Vec3 Hit::albedo()
{
    return material->albedo(0, 0);
}
util::Vec3 Hit::emission()
{
    return material->emission(0, 0);
}
}