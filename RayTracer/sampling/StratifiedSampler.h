#pragma once
#include "Sampler.h"
#include <random>
namespace util {
	class StratifiedSampler :public Sampler
	{
	public:
		StratifiedSampler (const std::shared_ptr<Sampler>& sampler, size_t n);
		Vec3 color (float x, float y) const;
	private:
		std::shared_ptr<Sampler> sampler_;
		size_t n_;
	};
	static std::random_device rd;
	static std::mt19937 gen (rd ());
	static std::uniform_real_distribution<>dis (0, 1.0);
}
