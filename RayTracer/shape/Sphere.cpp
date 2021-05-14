#define _USE_MATH_DEFINES

#include "Sphere.h"

#include <cassert>

#include "../tools/Random.h"
#include "math.h"

namespace shapes {
Sphere::Sphere(float radius,
               const std::shared_ptr<material::Material>& material)
    : radius(radius), material(material) {
}
std::optional<cam::Hit> Sphere::intersect(const cam::Ray& r) const {
	util::Vec3 d = r.d;
	util::Vec3 x0 = r.x0;
	float a = util::dot(d, d);
	float b = 2 * util::dot(x0, d);
	float c = util::dot(x0, x0) - (radius * radius);
	float discrim = b * b - 4 * a * c;

	if (discrim >= 0) {
		float t1 = (-b - sqrt(discrim)) / (2 * a);

		if (r.in_range(t1)) {
			util::Vec3 t1HitPoint = r(t1);
			return std::optional<cam::Hit>({t1HitPoint, t1HitPoint,
			                                texture_coordinates(t1HitPoint), t1,
			                                material});
		} else {
			float t2 = (-b + sqrt(discrim)) / (2 * a);
			if (r.in_range(t2)) {
				util::Vec3 t2HitPoint = r(t2);
				return std::optional<cam::Hit>({t2HitPoint, t2HitPoint,
				                                texture_coordinates(t2HitPoint),
				                                t2, material});
			} else {
				return std::nullopt;
			}
		}
	} else {
		return std::nullopt;
	}
}
std::pair<float, float> Sphere::texture_coordinates(
    const util::Vec3& pos) const {
	float theta = std::atan2(pos.x(), pos.z());
	float phi = std::acos(pos.y() / radius);
	return std::make_pair<float, float>((float)(theta / (2 * M_PI)),
	                                    (float)(phi / (M_PI)));
}
util::Vec3 Sphere::texture_coordinates(std::pair<float, float> texel) const {
	float theta = texel.first * M_PI * 2;
	float phi = texel.second * M_PI;
	float z = radius * std::cos(theta) * std::sin(phi);
	float x = radius * std::sin(theta) * std::sin(phi);
	float y = radius * std::cos(phi);
	return {x, y, z};
}
util::AxisAlignedBoundingBox Sphere::bounds() const {
	return util::AxisAlignedBoundingBox(util::Vec3(-radius),
	                                    util::Vec3(radius));
}
util::SurfacePoint Sphere::sampleLight(const cam::Hit& h) const {
	auto uv = material->sampleEmissionProfile();
	util::Vec3 point = texture_coordinates(uv);
	return util::SurfacePoint(point, point.normalize(), uv, material);
}
/*std::pair<util::Vec3, float> Sphere::calculateLightEmission(
    const util::SurfacePoint& p, const util::Vec3& d) const {
    // Basically this is just the emission at a surface point. And the pdf dimms
    // the light in regard to the angle.
    // Uniform pdf of shape is 1/area, converting to pdf over solid angle is
    // pdf/(dot/length^2).
    // This is wrong. We just need the normal pdf, per area, as we do not sample
    // with regard to a direction.
    auto emission = p.emission();
    // auto dot = std::max<float>(util::dot(p.normal(), d.normalize()), 0);
    auto area = 4 * M_PI * std::pow(radius, 2);
    auto uv = p.texel();
    float pdf = material->emission_pdf(uv.first, uv.second).value_or(1) / area;
    return {emission, pdf};
}*/

util::Vec3 Sphere::lightEmission(const util::SurfacePoint& p) const {
	return p.emission();
}
float Sphere::lightPdf(const util::SurfacePoint& p,
                       const util::Vec3& dl_out) const {
	auto dot = std::max<float>(util::dot(p.normal(), dl_out.normalize()), 0);

	auto uv = p.texel();
	auto phi = uv.second * M_PI;
	float pdf = material->emission_pdf(uv.first, uv.second).value_or(1);
	pdf = pdf / (dot / dl_out.length());
	return pdf;
}
}  // namespace shapes
