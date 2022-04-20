#pragma once

#include <vector>

#include "../camera/CamObs.h"
#include "Sampler.h"
#include "Scene.h"

namespace util {
class Image : public Sampler {
   public:
	void set_pixel(int x, int y, const Vec3& color);
	// Set all pixels with implemented threadpool
	void set_pixels(size_t threadcount, std::string fname, std::string formula,
	                std::shared_ptr<OptiSampler> sampler);

	Vec3 operator[](const std::array<int, 2>& i) const;  // int x, int y
	Vec3& operator[](const std::array<int, 2>& i);

	// Here x,y are in [0,1]
	Vec3 color(float x, float y) const override;

	const int width, height;

	void gamma_correct(float gamma);

	// Turn the images upper/lower half completly black.
	// Usefull for sky sphere, where you only need one hemisphere
	void half_image(bool upper, float tolerance);

	friend Image raytrace(size_t threadcount, std::string fname,
	                      std::string formula,
	                      const std::shared_ptr<Scene>& scene, size_t n);
	friend Image read_image(const char* filename);

   protected:
	// Task for the threadpools
	std::vector<int64_t> set_pixels_task(
	    int x, int y, const std::shared_ptr<OptiSampler>& sampler);

   private:
	std::vector<Vec3> vec;
	Image(int width, int height);
};

Image raytrace(size_t threadcount, std::string fname, std::string formula,
               const std::shared_ptr<Scene>& scene, size_t n);
void write_bmp(const char* filename, const Image& img);
Image read_image(const char* filename);

}  // namespace util