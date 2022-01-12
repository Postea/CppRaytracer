#include "StratifiedSampler.h"

namespace util {
StratifiedSampler::StratifiedSampler(const std::shared_ptr<Sampler>& sampler,
                                     size_t n)
    : sampler(sampler), n(std::ceil(std::sqrt(n))){};

Vec3 StratifiedSampler::color(float x, float y) const {
	Vec3 color(0, 0, 0);
	for (int xi = 0; xi < n; xi++) {
		for (int yi = 0; yi < n; yi++) {
			float rx = dis0to1(gen);
			float ry = dis0to1(gen);
			float xs = x + (xi + rx) / n;
			float ys = y + (yi + ry) / n;
			color = color + sampler->color(xs, ys);
		}
	}
	return color / (n * n);
}
}  // namespace util