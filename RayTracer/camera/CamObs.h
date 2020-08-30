#pragma once

#include "../tools/Mat4.h"
#include "Ray.h"

namespace cam {
class CamObs {
public:
    //Constructor
    CamObs(const util::Mat4& mat, float theta, int width, int height);
    //Methods
    Ray create(float x, float y) const;
    //Attributes
    const int width, height;

private:
    const util::Mat4 mat;
    const float theta;
};
}
