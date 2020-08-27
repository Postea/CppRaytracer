#include "Image.h"
#include "StratifiedSampler.h"

namespace util {
	Image::Image (int width, int height) :width_ (width), height_ (height) {
		Vec3 color ({});
		for (int i = 0; i < width * height; i++) {
			vec_.insert (vec_.end (), color);
		}
	}
	void Image::setPixels (std::shared_ptr<Sampler> sampler)
	{
		for (int x = 0; x != width_; x++) {
			for (int y = 0; y != height_; y++) {
				vec_[width_ * y + x] = sampler->color (x, y);
			}
		}
	}
	Vec3 Image::operator[] (const std::array<int, 2>& i) const {
		return vec_[width_ * i[1] + i[0]];
	}
	Vec3& Image::operator[] (const std::array<int, 2>& i) {
		return vec_[width_ * i[1] + i[0]];
	}
	Image raytrace (const cam::CamObs& cam, const std::shared_ptr<Sampler>& sampler, size_t n)
	{
		Image result(cam.width_, cam.height_);

		result.setPixels (
		std::make_shared<StratifiedSampler>(StratifiedSampler(sampler,n))
		//sampler
		);
		return result;
	}
}