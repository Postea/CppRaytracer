#pragma once
#include "../tools/Vec3.h"

namespace cam {
class Ray {
public:
    //Constructor
    Ray(const util::Vec3& x0, const util::Vec3& d, float tmin, float tmax, bool normalize);
    //Operator
    friend std::ostream& operator<<(std::ostream& os, const cam::Ray& rhs);
    util::Vec3 operator()(float t) const;
    //Methods
    bool in_range(float t) const;

    util::Vec3 x0_, d_;
    float tmin_, tmax_;
    bool normalize_;
};
}
