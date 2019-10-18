#pragma once
#include "Vec3.h"
#include "Sampler.h"
#include <vector>
#include "CamObs.h"

namespace util {
	class Image {
	public:
		Image (int width, int height);

		void setPixels (std::shared_ptr<Sampler> sampler);

		Vec3 operator[] ( const std::array<int, 2>& i) const; //int x, int y
		Vec3& operator[] (const std::array<int, 2>& i);

		const int width_;
		const int height_;
	private:
		std::vector<util::Vec3> vec_;
	};

	Image raytrace (const cam::CamObs& cam, const std::shared_ptr<Sampler>& sampler, size_t n);

}