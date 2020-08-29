#include "CamObs.h"
#include <limits>

namespace cam {
cam::CamObs::CamObs(const util::Mat4& mat, float theta, int width, int height)
    : mat_(mat)
    , theta_(theta)
    , width_(width)
    , height_(height)
{
}

Ray cam::CamObs::create(float x, float y) const
{
    util::Vec3 d(x - width_ / 2, height_ / 2 - y, -(width_ / 2) / (tan(theta_ / 2)));
    d = mat_.transformDir(d);

    return Ray(mat_.position(), d, 0, std::numeric_limits<float>::infinity(), true);
}
}
