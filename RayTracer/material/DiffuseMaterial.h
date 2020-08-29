#pragma once

#include "../sampling/Sampler.h"
#include "Material.h"
#include "texture/Constant.h"

namespace material {
class DiffuseMaterial : public Material {
public:
    DiffuseMaterial(const std::shared_ptr<util::Sampler>& albedo, const std::shared_ptr<util::Sampler>& emission);
    DiffuseMaterial(const std::shared_ptr<util::Sampler>& albedo);
    DiffuseMaterial(const util::Vec3& color);
    bool scatter();

    util::Vec3 albedo(float texel_x, float texel_y);
    util::Vec3 emission(float texel_x, float texel_y);
    util::Vec3 scattered_d(const util::Vec3& d, const util::Vec3& n);

private:
    std::shared_ptr<util::Sampler> emission_;
    std::shared_ptr<util::Sampler> albedo_;
};
}
