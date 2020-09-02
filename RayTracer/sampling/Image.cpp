#include "Image.h"

#include "../tools/Threadpool.h"
#include "StratifiedSampler.h"

namespace util {
Image::Image(int width, int height) : width(width), height(height) {
	Vec3 color({});
	for (int i = 0; i < width * height; i++) {
		vec.insert(vec.end(), color);
	}
}
/*
void Image::setPixels(std::shared_ptr<Sampler> sampler) {
    Threadpool tp(4);
    for (int x = 0; x != width; x++) {
        for (int y = 0; y != height; y++) {
            vec[width * y + x] = sampler->color(x, y);
        }
    }
} */
void Image::setPixel(int x, int y, Vec3 color) {
	vec[width * y + x] = color;
}
void Image::setPixels(size_t threadcount, std::shared_ptr<Sampler> sampler) {
	Threadpool tp(threadcount);
	for (int x = 0; x != width; x++) {
		for (int y = 0; y != height; y++) {
			tp.queueTask(std::bind([this, x, y, sampler]() {
				this->setPixelsTask(x, y, sampler);
			}));
		}
	}
	// std::cout << "Done queing" << std::endl;
}

void Image::setPixelsTask(int x, int y, std::shared_ptr<Sampler> sampler) {
	setPixel(x, y, sampler->color(x, y));
}
Vec3 Image::operator[](const std::array<int, 2>& i) const {
	return vec[width * i[1] + i[0]];
}
Vec3& Image::operator[](const std::array<int, 2>& i) {
	return vec[width * i[1] + i[0]];
}
Image raytrace(size_t threadcount, const cam::CamObs& cam,
               const std::shared_ptr<Sampler>& sampler, size_t n) {
	Image result(cam.width, cam.height);

	result.setPixels(threadcount, std::make_shared<StratifiedSampler>(
	                                  StratifiedSampler(sampler, n))
	                 // sampler
	);
	return result;
}
}  // namespace util