#include "StratifiedSampler.h"

namespace util {
StratifiedSampler::StratifiedSampler(const std::shared_ptr<Sampler>& sampler, size_t n)
    : sampler_(sampler)
    , n_(std::ceil(std::sqrt(n))) {};

Vec3 StratifiedSampler::color(float x, float y) const
{
    Vec3 color(0, 0, 0);
    for (int xi = 0; xi < n_; xi++) {
        for (int yi = 0; yi < n_; yi++) {
            double rx = dis(gen);
            double ry = dis(gen);
            double xs = x + (xi + rx) / n_;
            double ys = y + (yi + ry) / n_;
            color = color + sampler_->color(xs, ys);
        }
    }
    //if (color != Vec3 (16, 16, 16) && color != Vec3 (0, 0, 8))
    //std::cout << color << " and " << color / (n_ * n_) << n_ << std::endl;
    return color / (n_ * n_);
}
}