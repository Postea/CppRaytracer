#include "GridSampler.h"

namespace util {
GridSampler::GridSampler(const std::shared_ptr<Sampler>& sampler, size_t n)
    : sampler(sampler), n(std::ceil(std::sqrt(n))) {
}
Vec3 GridSampler::color(float x, float y) const {
	Vec3 color(0, 0, 0);
	for (int xi = 0; xi < n; xi++) {
		for (int yi = 0; yi < n; yi++) {
			double xs = x + (xi + 0.5) / n;
			double ys = y + (yi + 0.5) / n;
			color = color + sampler->color(xs, ys);
		}
	}
	return color / (n * n);
}
}  // namespace util