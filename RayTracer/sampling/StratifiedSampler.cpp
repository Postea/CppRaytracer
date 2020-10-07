#include "StratifiedSampler.h"

namespace util {
StratifiedSampler::StratifiedSampler(const std::shared_ptr<Sampler>& sampler,
                                     size_t n)
    : sampler(sampler), n(std::ceil(std::sqrt(n))){};

Vec3 StratifiedSampler::color(float x, float y) const {
	Vec3 color(0, 0, 0);
	for (int xi = 0; xi < n; xi++) {
		for (int yi = 0; yi < n; yi++) {
			float rx = dis(gen);
			float ry = dis(gen);
			float xs = x + (xi + rx) / n;
			float ys = y + (yi + ry) / n;
			color = color + sampler->color(xs, ys);
		}
	}
	// if (color != Vec3 (16, 16, 16) && color != Vec3 (0, 0, 8))
	// std::cout << color << " and " << color / (n_ * n_) << n_ << std::endl;
	return color / (n * n);
}
}  // namespace util