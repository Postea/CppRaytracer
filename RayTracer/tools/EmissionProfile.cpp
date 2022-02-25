#include "EmissionProfile.h"

#include <math.h>

#include <algorithm>
#include <numeric>

#include "../tools/Random.h"

namespace util {

EmissionProfile::EmissionProfile(const std::shared_ptr<util::Sampler>& emission,
                                 float intensity)
    : emission(emission),
      uniform(true),
      cdf(),
      intensity(intensity),
      normalizer(0.0f),
      distribution(std::nullopt){};
EmissionProfile::EmissionProfile(const std::shared_ptr<util::Sampler>& emission,
                                 const Image& distribution, float intensity)
    : emission(emission),
      uniform(false),
      intensity(intensity),
      distribution(std::make_optional(distribution)) {
	// vec: a vector with texel and grey value
	std::vector<std::pair<std::pair<float, float>, float>> vec;
	for (int w = 0; w < distribution.width; w++) {
		for (int h = 0; h < distribution.height; h++) {
			auto xyz = distribution[{w, h}];
			float grey = 0.299f * xyz[0] + 0.587f * xyz[1] + 0.114f * xyz[2];
			vec.push_back({{(float)w / (float)distribution.width,
			                (float)h / (float)distribution.height},
			               grey});
		}
	}
	// Sort the value from biggest to smallest v
	std::sort(vec.begin(), vec.end(),
	          [](std::pair<std::pair<float, float>, float> a,
	             std::pair<std::pair<float, float>, float> b) {
		          return a.second > b.second;
	          });
	// Remove all the pixels with zero probability
	std::remove_if(vec.begin(), vec.end(),
	               [](std::pair<std::pair<float, float>, float> a) {
		               return a.second == 0;
	               });
	// Build cdf
	cdf = {};
	float cumm = 0;
	for (auto x : vec) {
		cumm += x.second;
		cdf.push_back({x.first, cumm});
	}
	// Transform cdf to cap at 1
	for (int i = 0; i < cdf.size(); i++) {
		cdf[i].second = cdf[i].second / cumm;
	}
	// Normalizer
	normalizer = (distribution.width * distribution.height) / cumm;
};

std::pair<float, float> EmissionProfile::sample() const {
	if (uniform) {
		return {(float)util::dis0to1(util::gen),
		        (float)util::dis0to1(util::gen)};
	} else {
		auto s = dis0to1(gen);
		auto res = binary_search(s);
		return res.first;
	}
};
util::Vec3 EmissionProfile::color(float x, float y) const {
	return emission->color(x, y);
}
std::optional<float> EmissionProfile::pdf(float x, float y) const {
	if (uniform) return std::nullopt;
	auto xyz = distribution->color(x, y);

	return std::make_optional<float>(
	    (0.299f * xyz[0] + 0.587f * xyz[1] + 0.114f * xyz[2]) * normalizer);
}

std::pair<std::pair<float, float>, float> EmissionProfile::binary_search(
    float target, int start, int end) const {
	int mid;
	if (end == -1) end = cdf.size();
	while (start <= end) {
		mid = (start + end) / 2;
		if (cdf[mid].second <= target)
			start = mid + 1;
		else
			end = mid - 1;
	}
	return cdf[mid];
}
// Not used
std::pair<std::pair<float, float>, float> EmissionProfile::exponential_search(
    float target) const {
	if (cdf[0].second >= target) return cdf[0];
	int start = 1;
	while (start < cdf.size() && cdf[start].second <= target) start = 2 * start;
	return binary_search(target, start / 2, std::min<int>(start, cdf.size()));
}
}  // namespace util