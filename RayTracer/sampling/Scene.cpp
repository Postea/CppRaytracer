#define _USE_MATH_DEFINES
#include "Scene.h"

#include <math.h>

#include <cassert>

Scene::Scene(const shapes::Group& group,
             const std::vector<std::shared_ptr<shapes::Light>>& lights,
             const cam::CamObs& cam, size_t max_depth, size_t light_n)
    : group(group),
      lights(lights),
      cam(cam),
      max_depth(max_depth),
      light_n(light_n),
      direct(light_n != 0) {
}

util::Vec3 Scene::color(float x, float y) const {
	cam::Ray r = cam.create(x, y);
	return calculate_radiance(r, max_depth);
}

util::Vec3 Scene::calculate_radiance(const cam::Ray& r, size_t depth) const {
	// Recursion limit hit
	if (depth == 0) return util::Vec3(0);

	std::optional<cam::Hit> h = group.intersect(r);
	assert(h);
	auto scatter_ray = h->scattered_ray(r);
	// If the material does not scatter, return
	if (!scatter_ray) {
		// if we are at the first depth, we have to count the materials emission
		// as we did not account for it with direct_lightning
		if (direct) {
			if (max_depth == depth) return h->emission();
			return util::Vec3(0);
		} else
			return h->emission();
	}
	auto brdf = h->brdf(-scatter_ray->d, -r.d, h->normal());
	auto L_i = calculate_radiance(scatter_ray.value(), depth - 1);
	auto cosine_term = util::dot(h->normal(), scatter_ray->d.normalize());

	auto brdf_pdf = h->material->brdf_pdf(scatter_ray->d, h->normal());

	auto scatter_function = (brdf * L_i * cosine_term) / brdf_pdf;

	util::Vec3 L_direct;
	util::Vec3 L_e;
	if (direct) {
		L_direct = direct_lighting(h.value(), r);
		L_e = (max_depth == depth) ? h->emission() : util::Vec3(0);
	} else {
		L_direct = util::Vec3(0);
		L_e = util::Vec3(0);
	}
	return L_e + (h->albedo() * (scatter_function + L_direct));
}
util::Vec3 Scene::direct_lighting(const cam::Hit& h, const cam::Ray& r) const {
	util::Vec3 result;
	for (const auto& light : lights) {
		util::Vec3 light_part;
		for (int i = 0; i < light_n; i++) {
			auto sample_point = light->sample_light(h);
			auto shadow_ray =
			    cam::Ray(sample_point.point(), h.point() - sample_point.point(),
			             cam::epsilon, 1 - cam::epsilon, false);

			// When the surface normal and the shadowray dont point in the same
			// hemisphere, the ray hits its own surface, so we can continue
			// This is part of the light_pdf and with this condition, the light
			// pdf should never be able to be < 0
			if (util::dot(shadow_ray.d, sample_point.normal()) <= 0) continue;
			// Dot product could be negative, in that case continue
			auto cosine_term = util::dot(-shadow_ray.d.normalize(), h.normal());
			if (cosine_term <= 0) continue;
			auto shadow_hit = group.intersect(shadow_ray);
			// If the shadowray his something we can continue
			if (shadow_hit) continue;

			auto light_pdf = light->light_pdf(sample_point, shadow_ray.d);
			auto brdf = h.brdf(shadow_ray.d.normalize(), -r.d, h.normal());
			auto L = light->light_emission(sample_point);

			auto scatter_function = (brdf * L * cosine_term) / light_pdf;
			// Add the values from this light to the others
			light_part = light_part + scatter_function;
		}
		result = result + (light_part / light_n);
	}
	return result;
}
