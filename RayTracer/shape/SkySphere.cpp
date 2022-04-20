#include "SkySphere.h"
#define _USE_MATH_DEFINES
#include <cassert>
#include <limits>

#include "../material/Emitting.h"
#include "math.h"

namespace shapes {
SkySphere::SkySphere(const std::shared_ptr<util::Sampler>& sampler)
    : Sphere(1000, std::make_shared<material::Emitting>(sampler)) {
}
SkySphere::SkySphere(const std::shared_ptr<util::Sampler>& sampler,
                     util::Image& distribution, float intensity)
    : Sphere(1000, std::make_shared<material::Emitting>(sampler)) {
	for (int x = 0; x < distribution.width; x++) {
		for (int y = 0; y < distribution.height; y++) {
			auto color = distribution[{x, y}];
			auto sinn = sin(M_PI * ((float)y / distribution.height));
			assert(sinn >= 0);
			color = color * sinn;
			distribution.set_pixel(x, y, color);
		}
	}
	material =
	    std::make_shared<material::Emitting>(sampler, distribution, intensity);
}

util::SurfacePoint SkySphere::sample_light(const cam::Hit& h) const {
	auto uv = material->sample_emission_profile();
	util::Vec3 point = texture_coordinates(uv);
	return util::SurfacePoint(point, -point.normalize(), uv, material);
}

util::Vec3 SkySphere::light_emission(const util::SurfacePoint& p) const {
	return p.emission();
}

float SkySphere::light_pdf(const util::SurfacePoint& p,
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