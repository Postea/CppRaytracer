#define _USE_MATH_DEFINES
#include "Scene.h"

#include <math.h>

#include <cassert>

Scene::Scene(const shapes::Group& group,
             const std::vector<std::shared_ptr<shapes::Light>> lights,
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
	return calculateRadiance(r, max_depth);
}

util::Vec3 Scene::calculateRadiance(const cam::Ray& r, size_t depth) const {
	// Recursion limit hit
	if (depth == 0) return util::Vec3(0);

	std::optional<cam::Hit> h = group.intersect(r);
	auto scatter_ray = h->scattered_ray(r);
	// If the material does not scatter, return
	if (!scatter_ray) {
		// if we are at the first depth, we have to count the materials emission
		// as we did not account for it with directLightning
		if (direct) {
			if (this->max_depth == depth) return h->emission();
			return util::Vec3(0);
		} else
			return h->emission();
	}
	auto brdf = h->calculateLightMultiplier(-scatter_ray->d, -r.d, h->normal());
	auto L = calculateRadiance(scatter_ray.value(), depth - 1);
	auto cosine_term = util::dot(h->normal(), scatter_ray->d.normalize());
	// This can not happen
	if (cosine_term <= 0) {
		std::cout << "hm" << std::endl;
	}
	auto brdf_pdf = h->material->brdf_pdf(scatter_ray->d, h->normal());

	auto scatter_function = (brdf * L * cosine_term) / brdf_pdf;

	util::Vec3 L_direct;
	if (direct)
		L_direct = directLighting(h, r, light_n);
	else
		L_direct = util::Vec3(0);

	return h->albedo() * (scatter_function + L_direct);
}
util::Vec3 Scene::directLighting(const std::optional<cam::Hit>& h, cam::Ray r,
                                 int n) const {
	util::Vec3 result;
	for (auto light : lights) {
		util::Vec3 light_part;
		for (int i = 0; i < n; i++) {
			auto sample_point = light->sampleLight(h.value());
			auto shadow_ray = cam::Ray(sample_point.point(),
			                           h->point() - sample_point.point(),
			                           cam::epsilon, 1 - cam::epsilon, false);
			// When the surface normal and the shadowray dont point in the same
			// hemisphere, the ray hits the surface, so we can break
			if (util::dot(shadow_ray.d, sample_point.normal()) <= 0) break;

			auto shadow_hit = group.intersect(shadow_ray);
			// If the shadowray his something we can break
			if (shadow_hit) break;
			auto light_pdf = light->lightPdf(sample_point, shadow_ray.d);
			// This happens when the shaodow ray is beyond the surfaces normal.
			// This means, that the shadow ray hits the surface and we can break
			if (light_pdf <= 0) break;
			auto brdf = h->calculateLightMultiplier(shadow_ray.d.normalize(),
			                                        -r.d, h->normal());
			auto L = light->lightEmission(sample_point);
			// Dot product could be negative, in that case break
			auto cosine_term = std::max<float>(
			    util::dot(-shadow_ray.d.normalize(), h->normal()), 0);
			if (cosine_term <= 0) break;

			auto scatterFunction = (brdf * L * cosine_term) / light_pdf;
			// Add the values from this light to the others
			light_part = light_part + scatterFunction;
		}
		result = result + (light_part / n);
	}
	return result;
}
