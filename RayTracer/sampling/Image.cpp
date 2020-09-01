#include "Image.h"

#include "StratifiedSampler.h"

namespace util {
Image::Image(int width, int height) : width(width), height(height) {
	Vec3 color({});
	for (int i = 0; i < width * height; i++) {
		vec.insert(vec.end(), color);
	}
}
void Image::setPixels(std::shared_ptr<Sampler> sampler) {
	for (int x = 0; x != width; x++) {
		for (int y = 0; y != height; y++) {
			vec[width * y + x] = sampler->color(x, y);
		}
	}
}
Vec3 Image::operator[](const std::array<int, 2>& i) const {
	return vec[width * i[1] + i[0]];
}
Vec3& Image::operator[](const std::array<int, 2>& i) {
	return vec[width * i[1] + i[0]];
}
Image raytrace(const cam::CamObs& cam, const std::shared_ptr<Sampler>& sampler,
               size_t n) {
	Image result(cam.width, cam.height);

	result.setPixels(
	    std::make_shared<StratifiedSampler>(StratifiedSampler(sampler, n))
	    // sampler
	);
	return result;
}
}  // namespace util