#include "BackgroundMaterial.h"
#include "Constant.h"

namespace material {
	BackgroundMaterial::BackgroundMaterial (const std::shared_ptr<util::Sampler>& texture)
		:texture_ (texture) {}
	BackgroundMaterial::BackgroundMaterial (const util::Vec3& color)
		: texture_ (std::make_shared<Constant> (color)) {}
	util::Vec3 BackgroundMaterial::albedo (float texel_x, float texel_y)
	{
		return util::Vec3 (1, 1, 1);
	}
	util::Vec3 BackgroundMaterial::emission (float texel_x, float texel_y)
	{
		return texture_->color (texel_x, -texel_y);
	}
	util::Vec3 BackgroundMaterial::scattered_d (const util::Vec3& d, const util::Vec3& n)
	{
		return util::Vec3 (0, 0, 0);
	}
	bool BackgroundMaterial::scatter ()
	{
		return false;
	}
}