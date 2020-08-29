#pragma once

#include "../../sampling/Sampler.h"
#include "../../tools/Vec3.h"

namespace material {
class Constant : public util::Sampler {
private:
    const util::Vec3 color_;

public:
    Constant(const util::Vec3& color);

    util::Vec3 color(float x, float y) const override;
};
}
