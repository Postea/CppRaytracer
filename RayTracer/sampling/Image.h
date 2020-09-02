#pragma once

#include <vector>

#include "../camera/CamObs.h"
#include "../tools/Vec3.h"
#include "Sampler.h"

namespace util {
class Image {
   public:
	Image(int width, int height);

	void setPixel(int x, int y, Vec3 color);
	void setPixels(std::shared_ptr<Sampler> sampler);

	Vec3 operator[](const std::array<int, 2>& i) const;  // int x, int y
	Vec3& operator[](const std::array<int, 2>& i);

	const int width;
	const int height;

   private:
	std::vector<Vec3> vec;
};

Image raytrace(const cam::CamObs& cam, const std::shared_ptr<Sampler>& sampler,
               size_t n);

}  // namespace util