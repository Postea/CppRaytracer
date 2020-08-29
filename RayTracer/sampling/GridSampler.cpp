#include "GridSampler.h"

namespace util {
GridSampler::GridSampler(const std::shared_ptr<Sampler>& sampler, size_t n)
    : sampler_(sampler)
    , n_(std::ceil(std::sqrt(n)))
{
}
Vec3 GridSampler::color(float x, float y) const
{
    Vec3 color(0, 0, 0);
    for (int xi = 0; xi < n_; xi++) {
        for (int yi = 0; yi < n_; yi++) {
            double xs = x + (xi + 0.5) / n_;
            double ys = y + (yi + 0.5) / n_;
            color = color + sampler_->color(xs, ys);
        }
    }
    return color / (n_ * n_);
}
}