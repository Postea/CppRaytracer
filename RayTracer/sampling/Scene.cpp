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
		result = h->emission() +
		         h->albedo() * calculateRadiance(scatter_ray, depth - 1, write);

		// To investigate possible bugs
		/*
		    if (write) {
		        std::cout << depth << " " << r << " " << r.tmin << std::endl;
		        std::cout << *h << std::endl;
		        if (depth == 20) assert(false);
		    }*/
#if false
		for (auto light : lights) {
			int nn = 5;
			nn = nn * nn;
			util::Vec3 lightPart;
			for (int i = 0; i < nn; i++) {
				auto samplePoint = light->sampleLight();
				auto shadowRay = cam::Ray(samplePoint.point(),
				                          h->point() - samplePoint.point(), 0,
				                          1 - cam::epsilon, false);
				auto shadowHit = group.intersect(shadowRay);
				if (!shadowHit) {
					auto lightEmission =
					    light->calculateLightEmission(samplePoint, shadowRay.d);
					auto lightMultiplier = h->calculateLightMultiplier(
					    shadowRay.d.normalize(), -r.d, h->normal());

					util::Vec3 scatterFunction =
					    lightMultiplier * lightEmission *
					    std::max<float>(
					        util::dot(-shadowRay.d.normalize(), h->normal()),
					        0);
					lightPart = lightPart + (scatterFunction);
				}
			}
			result = result + (h->albedo() * (lightPart / nn));
		}
#endif

	} else {
		result = h->emission();
	}
	return result;
	// return h->normal();
}
