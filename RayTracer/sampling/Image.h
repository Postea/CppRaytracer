#pragma once

#include <vector>

#include "../camera/CamObs.h"
#include "Sampler.h"

namespace util {
class Image : public Sampler {
   public:
	Image(int width, int height);

	void setPixel(int x, int y, Vec3 color);
	void setPixels(size_t threadcount, std::shared_ptr<Sampler> sampler);

	Vec3 operator[](const std::array<int, 2>& i) const;  // int x, int y
	Vec3& operator[](const std::array<int, 2>& i);

	Vec3 color(float x, float y) const override;

	const int width;
	const int height;
	void gammaCorrect(float gamma);

   protected:
	void setPixelsTask(int x, int y, std::shared_ptr<Sampler> sampler);

   private:
	std::vector<Vec3> vec;
};

Image raytrace(size_t threadcount, const cam::CamObs& cam,
               const std::shared_ptr<Sampler>& sampler, size_t n);
void writeBmp(const char* filename, Image img);
Image readImage(const char* filename);

}  // namespace util