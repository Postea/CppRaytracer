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
	return calculateRadiance(r, depth);
}

util::Vec3 Scene::calculateRadiance(const cam::Ray& r, size_t depth) const {
	if (depth == 0) {
		return util::Vec3(0, 0, 0);
	}
	std::optional<cam::Hit> h = group.intersect(r);

	util::Vec3 result;  // = h->emission();
	std::optional<cam::Ray> scatter = h->scattered_ray(r);
	if (scatter) {
		util::Vec3 result;
		for (auto light : lights) {
			util::Vec3 lightPart;
			for (int i = 0; i < 1; i++) {
				// Sample the light source with mis
				auto samplePoint = light->sampleLight(h.value());
				auto shadowRay = cam::Ray(
				    samplePoint.point(), h->point() - samplePoint.point(),
				    cam::epsilon, 1 - cam::epsilon, false);
				// Only check for intersection when the the ray points away from
				// the surface of the light. otherwise it hits the light itself
				// and we can break
				if (util::dot(shadowRay.d, samplePoint.normal()) <= 0) break;
				auto shadowHit = group.intersect(shadowRay);
				// If there is a hit from the shadowRay break
				if (shadowHit) break;
				auto lightPdf = light->lightPdf(samplePoint, shadowRay.d);
				// Can this even happen?
				if (lightPdf <= 0) {
					std::cout << "Hm" << std::endl;
					break;
				}
				auto brdf = h->calculateLightMultiplier(shadowRay.d.normalize(),
				                                        -r.d, h->normal());
				auto brdf_pdf =
				    h->material->brdf_pdf(-shadowRay.d, h->normal());
				auto L = light->lightEmission(samplePoint);
				auto cosine_term = std::max<float>(
				    0, util::dot(-shadowRay.d.normalize(), h->normal()));
				if (cosine_term < 0) {
					std::cout << "hm" << std::endl;
				}
				auto weight = misWeight(1, lightPdf, 1, brdf_pdf);

				util::Vec3 scatterFunction =
				    (brdf * L * cosine_term * weight) / lightPdf;

				lightPart = lightPart + (scatterFunction);

				// Sample the brdf with mis
				auto brdf_ray = h->scattered_ray(r);
				auto lh = light->intersect(brdf_ray.value());
				if (!lh) break;
				brdf = lh->calculateLightMultiplier(-brdf_ray->d, -r.d,
				                                    h->normal());
				brdf_pdf = h->material->brdf_pdf(brdf_ray->d, h->normal());
				L = light->lightEmission(lh.value()) +
				    calculateRadiance(brdf_ray.value(), depth - 1);
				lightPdf = light->lightPdf(lh.value(), -brdf_ray->d);
				cosine_term = util::dot(brdf_ray->d, h->normal());
				weight = misWeight(1, brdf_pdf, 1, lightPdf);

				scatterFunction = (brdf * L * cosine_term * weight) / brdf_pdf;

				lightPart = lightPart + (scatterFunction);
			}
			result = result + (h->albedo() * (lightPart / 1));
		}
		return result;
	} else {
		// if (this->depth == depth) result = h->emission();
		result = h->emission();
	}
	return result;
}
util::Vec3 Scene::directLighting(const std::optional<cam::Hit>& h, cam::Ray r,
                                 int samples) const {
	util::Vec3 result;
	for (auto light : lights) {
		util::Vec3 lightPart;
		for (int i = 0; i < samples; i++) {
			// Sample the light source with mis
			auto samplePoint = light->sampleLight(h.value());
			auto shadowRay =
			    cam::Ray(samplePoint.point(), h->point() - samplePoint.point(),
			             cam::epsilon, 1 - cam::epsilon, false);
			// Only check for intersection when the the ray points away from the
			// surface of the light. otherwise it hits the light itself and we
			// can break
			if (util::dot(shadowRay.d, samplePoint.normal()) <= 0) break;
			auto shadowHit = group.intersect(shadowRay);
			// If there is a hit from the shadowRay break
			if (shadowHit) break;
			auto lightPdf = light->lightPdf(samplePoint, shadowRay.d);
			// Can this even happen?
			if (lightPdf <= 0) {
				std::cout << "Hm" << std::endl;
				break;
			}
			auto brdf = h->calculateLightMultiplier(shadowRay.d.normalize(),
			                                        -r.d, h->normal());
			auto brdf_pdf = h->material->brdf_pdf(-shadowRay.d, h->normal());
			auto L = light->lightEmission(samplePoint);
			auto cosine_term = util::dot(-shadowRay.d.normalize(), h->normal());
			if (cosine_term < 0) {
				std::cout << "hm" << std::endl;
			}
			auto weight = misWeight(1, lightPdf, 1, brdf_pdf);
			// std::cout << "light:" << lightPdf << " " << brdf_pdf << " "
			//           << weight << std::endl;
			util::Vec3 scatterFunction = (brdf * L * cosine_term) / lightPdf;

			lightPart = lightPart + (scatterFunction);

			// Sample the brdf with mis
			auto brdf_ray = h->scattered_ray(r);
			auto lh = light->intersect(brdf_ray.value());
			if (!lh) break;
			brdf =
			    lh->calculateLightMultiplier(-brdf_ray->d, -r.d, h->normal());
			brdf_pdf = h->material->brdf_pdf(brdf_ray->d, h->normal());
			L = light->lightEmission(lh.value());
			lightPdf = light->lightPdf(lh.value(), -brdf_ray->d);
			cosine_term = util::dot(brdf_ray->d, h->normal());
			weight = misWeight(1, brdf_pdf, 1, lightPdf);
			// std::cout << "brdf:" << brdf_pdf << " " << lightPdf << " " <<
			// weight
			//           << std::endl;
			scatterFunction = (brdf * L * cosine_term) / brdf_pdf;

			lightPart = lightPart + (scatterFunction);
		}
		result = result + (h->albedo() * (lightPart / samples));
	}
	return result;
}
// Not used
/*float Scene::bsdfMisWeight(const std::optional<cam::Hit>& h,
                           const cam::Ray& scatterRay,
                           size_t lights_samples) const {
    float numerator = h->material->brdf_pdf(scatterRay.d, h->normal());
    float denominator = numerator;
    for (auto light : lights) {
        //auto light_hit = light->intersect(scatterRay);
        //if (light_hit)
        //    denominator += lights_samples *
        //                   light->lightPdf(light_hit.value(), -scatterRay.d);
    }
    return numerator / denominator;
}
*/
float Scene::misWeight(size_t f_samples, float f_pdf, size_t g_samples,
                       float g_pdf) const {
	float li = f_samples * f_pdf;
	float br = g_samples * g_pdf;
	return (li * li) / ((li * li) + (br * br));
}
