#include "CirclePlane.h"

namespace shapes {
CirclePlane::CirclePlane(float radius, const std::shared_ptr<material::Material>& material)
    : radius_(radius)
    , material_(material)
{
}
std::shared_ptr<cam::Hit> CirclePlane::intersect(const cam::Ray& r)
{
    util::Vec3 n(0, 1, 0);
    util::Vec3 x0 = r.x0_;
    util::Vec3 d = r.d_;

    if (util::dot(d, n) > 0) {
        n = n * -1;
    }
    float a = util::dot(d, n);
    if (a == 0) {
        return nullptr;
    } else {
        float t = -x0[1] / d[1];
        util::Vec3 t_hitpoint = r(t);

        if (r.in_range(t) && t_hitpoint.length() <= radius_) {
            return std::make_shared<cam::Hit>(cam::Hit(t_hitpoint, n, t, material_));
        } else {
            return nullptr;
        }
    }
}
}
