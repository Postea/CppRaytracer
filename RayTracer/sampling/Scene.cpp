#include "Scene.h"

Scene::Scene(const shapes::Group& group, const shapes::Background& bg,
             const cam::CamObs& cam, size_t depth)
    : group(group), bg(bg), cam(cam), depth(depth) {
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
	util::Vec3 result;
	if (!h) {
		h = bg.intersect(r);
		return h->emission();
	}
	if (h->scatter()) {
		result =
		    h->albedo() * calculateRadiance(h->scattered_ray(r), depth - 1);
	} else {
		result = util::Vec3(0, 0, 0);
	}
	return result;
}
