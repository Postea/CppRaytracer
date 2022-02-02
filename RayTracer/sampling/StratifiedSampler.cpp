#include "StratifiedSampler.h"

namespace util {
StratifiedSampler::StratifiedSampler(
    const std::shared_ptr<OptiSampler>& sampler, size_t n)
    : sampler(sampler), n(std::ceil(std::sqrt(n))){};

std::pair<Vec3, std::vector<int64_t>> StratifiedSampler::color_opti(
    float x, float y) const {
	Vec3 color(0, 0, 0);
	std::vector<int64_t> paths;
	for (int xi = 0; xi < n; xi++) {
		for (int yi = 0; yi < n; yi++) {
			float rx = dis0to1(gen);
			float ry = dis0to1(gen);
			float xs = x + (xi + rx) / n;
			float ys = y + (yi + ry) / n;
			auto color_pair = sampler->color_opti(xs, ys);
			color = color + color_pair.first;
			// Stats
			auto prim = color_pair.second.at(0);
			auto dire = color_pair.second.at(1);
			paths.push_back(prim);
			paths.push_back(dire);
			paths.push_back(-1);
		}
	}
	return {color / (n * n), paths};
}
}  // namespace util