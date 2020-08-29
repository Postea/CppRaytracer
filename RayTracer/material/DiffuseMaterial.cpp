#include "DiffuseMaterial.h"
#include "../tools/Vec3.h"
#include "texture/Constant.h"

namespace material {
DiffuseMaterial::DiffuseMaterial(const std::shared_ptr<util::Sampler>& albedo, const std::shared_ptr<util::Sampler>& emission)
    : albedo_(albedo)
    , emission_(emission)
{
}
DiffuseMaterial::DiffuseMaterial(const std::shared_ptr<util::Sampler>& albedo)
    : albedo_(albedo)
    , emission_(std::make_shared<Constant>(Constant(util::Vec3(0, 0, 0))))
{
}
DiffuseMaterial::DiffuseMaterial(const util::Vec3& color)
    : albedo_(std::make_shared<Constant>(Constant(color)))
    , emission_(std::make_shared<Constant>(Constant(util::Vec3(0, 0, 0))))
{
}
util::Vec3 DiffuseMaterial::albedo(float texel_x, float texel_y)
{
    return albedo_->color(texel_x, texel_y);
}
util::Vec3 DiffuseMaterial::emission(float texel_x, float texel_y)
{
    return emission_->color(texel_x, texel_y);
}
util::Vec3 DiffuseMaterial::scattered_d(const util::Vec3& d, const util::Vec3& n)
{
    return n + rand_vec3_in_circle(1);
}
bool DiffuseMaterial::scatter()
{
    return true;
}
}