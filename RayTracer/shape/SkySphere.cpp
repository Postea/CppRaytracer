#include "SkySphere.h"
#define _USE_MATH_DEFINES
#include <cassert>
#include <limits>

#include "../material/BackgroundMaterial.h"
#include "math.h"

namespace shapes {
SkySphere::SkySphere(const std::shared_ptr<util::Sampler>& sampler)
    : Sphere(1000, std::make_shared<material::BackgroundMaterial>(sampler)) {
}
SkySphere::SkySphere(const std::shared_ptr<util::Sampler>& sampler,
                     util::Image& distribution, float intensity)
    : Sphere(1000, std::make_shared<material::BackgroundMaterial>(sampler)) {
	for (int x = 0; x < distribution.width; x++) {
		for (int y = 0; y < distribution.height; y++) {
			auto color = distribution[{x, y}];
			auto sinn = sin(M_PI * ((float)y / distribution.height));
			assert(sinn >= 0);
			color = color * sinn;
			distribution.setPixel(x, y, color);
		}
	}
	material = std::make_shared<material::BackgroundMaterial>(
	    sampler, distribution, intensity);
}
// This intersect method rightly flips the normal.But the normal is never used
// for non scatter materials, so we do not flip the normal
/*
std::optional<cam::Hit> Background::intersect(const cam::Ray& r) const {
    auto hit = Sphere::intersect(r);
    if (hit)
        hit = std::optional<cam::Hit>(
            {{hit->point(), -hit->normal(), hit->texel(), hit->scalar(),
              hit->material}});
    return hit;
}*/

util::SurfacePoint SkySphere::sampleLight(const cam::Hit& h) const {
	auto uv = material->sampleEmissionProfile();
	util::Vec3 point = texture_coordinates(uv);
	return util::SurfacePoint(point, -point.normalize(), uv, material);
}

float SkySphere::lightPdf(const util::SurfacePoint& p,
                          const util::Vec3& dl_out) const {
	auto uv = p.texel();
	auto phi = uv.second * M_PI;
	float pdf = material->emission_pdf(uv.first, uv.second).value();
	pdf = pdf / (2 * M_PI * M_PI * sin(phi));

	return pdf;
}

util::AxisAlignedBoundingBox SkySphere::bounds() const {
	return util::AxisAlignedBoundingBox();
}
}  // namespace shapes