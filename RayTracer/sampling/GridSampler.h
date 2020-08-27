#pragma once
#include "Sampler.h"

namespace util {
	class GridSampler :public Sampler
	{
	public:
		GridSampler (const std::shared_ptr<Sampler>& sampler, size_t n);
		Vec3 color (float x, float y) const;
	private:
		std::shared_ptr<Sampler> sampler_;
		size_t n_;
	};
}
