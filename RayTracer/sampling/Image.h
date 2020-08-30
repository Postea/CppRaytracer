#pragma once

#include "../camera/CamObs.h"
#include "../tools/Vec3.h"
#include "Sampler.h"
#include <vector>

namespace util {
class Image {
public:
    Image(int width, int height);

    void setPixels(std::shared_ptr<Sampler> sampler);

    Vec3 operator[](const std::array<int, 2>& i) const; //int x, int y
    Vec3& operator[](const std::array<int, 2>& i);

    const int width;
    const int height;

private:
    std::vector<util::Vec3> vec;
};

Image raytrace(const cam::CamObs& cam, const std::shared_ptr<Sampler>& sampler, size_t n);

}