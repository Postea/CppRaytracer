#include "Stratified.h"

namespace util {
Stratified::Stratified(const std::shared_ptr<Sampler>& sampler, size_t n)
    : sampler(sampler), n(n){};

Vec3 Stratified::color(float x, float y) const {
	Vec3 color(0, 0, 0);
	for (int xi = 0; xi < n; xi++) {
		for (int yi = 0; yi < n; yi++) {
			float rx = dis_0_to_1(gen);
			float ry = dis_0_to_1(gen);
			float xs = x + (xi + rx) / n;
			float ys = y + (yi + ry) / n;
			color = color + sampler->color(xs, ys);
		}
	}
	return color / (n * n);
}
}  // namespace util