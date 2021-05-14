#define _USE_MATH_DEFINES
#include "Scene.h"

#include <math.h>

#include <cassert>

Scene::Scene(const shapes::Group& group,
             const std::vector<std::shared_ptr<shapes::Light>> lights,
             const cam::CamObs& cam, size_t depth)
    : group(group), lights(lights), cam(cam), depth(depth) {
}

util::Vec3 Scene::color(float x, float y) const {
	cam::Ray r = cam.create(x, y);
	bool l = false;
	return calculateRadiance(r, depth, l);
}

util::Vec3 Scene::calculateRadiance(const cam::Ray& r, size_t depth,
                                    bool& write) const {
	if (depth == 0) {
		return util::Vec3(0, 0, 0);
	}
	std::optional<cam::Hit> h = group.intersect(r);
	// To investigate possible bugs
	/*if (depth < 19) {
	    write = true;
	    std::cout << depth << " " << r << std::endl;
	    std::cout << *h << std::endl;
	    // assert(false);
	}*/
	util::Vec3 result;
	cam::Ray scatter_ray = h->scattered_ray(r);

	if (h->scatter(scatter_ray.d, h->normal())) {
		size_t lightSamples = 10;
		// lightSamples = lightSamples * lightSamples;
		auto cosine_term = util::dot(h->normal(), scatter_ray.d.normalize());
		auto brdf = h->material->calculateLightMultiplier(-scatter_ray.d, -r.d,
		                                                  h->normal());
		auto brdf_pdf = h->material->brdf_pdf(scatter_ray.d, h->normal());
		result =
		    h->emission() +
		    (h->albedo() * calculateRadiance(scatter_ray, depth - 1, write) *
		     brdf * cosine_term) /
		        brdf_pdf;

		// To investigate possible bugs
		/*
		    if (write) {
		        std::cout << depth << " " << r << " " << r.tmin << std::endl;
		        std::cout << *h << std::endl;
		        if (depth == 20) assert(false);
		    }*/
#if true
		result = result + directLighting(h, r, lightSamples);
#endif
	} else {
		if (this->depth == depth) result = h->emission();
		// result = util::Vec3(0);
		// result = h->emission();
	}
	return result;
	// return h->normal();
}
util::Vec3 Scene::directLighting(const std::optional<cam::Hit>& h, cam::Ray r,
                                 int lightSamples) const {
	util::Vec3 result;
	for (auto light : lights) {
		util::Vec3 lightPart;
		for (int i = 0; i < lightSamples; i++) {
			auto samplePoint = light->sampleLight(h.value());
			auto shadowRay =
			    cam::Ray(samplePoint.point(), h->point() - samplePoint.point(),
			             cam::epsilon, 1 - cam::epsilon, false);
			auto shadowHit = group.intersect(shadowRay);
			if (!shadowHit) {
				auto lightPdf = light->lightPdf(samplePoint, shadowRay.d);
				if (lightPdf > 0) {
					auto lightEmission =
					    light->lightEmission(samplePoint) / lightPdf;
					auto lightMultiplier = h->calculateLightMultiplier(
					    shadowRay.d.normalize(), -r.d, h->normal());
					util::Vec3 scatterFunction =
					    lightMultiplier * lightEmission *
					    std::max<float>(
					        util::dot(-shadowRay.d.normalize(), h->normal()),
					        0);
					lightPart = lightPart + (scatterFunction);
				} else {
					result = result;
				}
			}
		}
		result = result + (h->albedo() * (lightPart / lightSamples));
	}
	return result;
}
float Scene::bsdfMisWeight(const std::optional<cam::Hit>& h,
                           const cam::Ray& scatterRay,
                           size_t lights_samples) const {
	float numerator = h->material->brdf_pdf(scatterRay.d, h->normal());
	float denominator = numerator;
	for (auto light : lights) {
		/*auto light_hit = light->intersect(scatterRay);
		if (light_hit)
		    denominator += lights_samples *
		                   light->lightPdf(light_hit.value(), -scatterRay.d);*/
	}
	return numerator / denominator;
}
float Scene::lightMisWeight(std::shared_ptr<shapes::Light> light,
                            const std::optional<cam::Hit>& h,
                            const util::Vec3& d_in,
                            const util::SurfacePoint& samplePoint,
                            size_t light_samples) const {
	float numerator = light_samples * light->lightPdf(samplePoint, d_in);
	float denominator = numerator + h->material->brdf_pdf(-d_in, h->normal());
	return numerator / denominator;
}
