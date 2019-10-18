#include "Scene.h"

Scene::Scene (std::shared_ptr<shapes::Shape> group, cam::CamObs cam, size_t depth)
	:group_ (group), cam_ (cam), depth_ (depth) {}

util::Vec3 Scene::color (float x, float y) const
{
	cam::Ray r = cam_.create (x, y);
	return calculateRadiance (r, depth_);
}

util::Vec3 Scene::calculateRadiance (const cam::Ray& r, size_t depth) const
{
	if (depth == 0) {
		return util::Vec3 (0, 0, 0);
	}
	std::shared_ptr<cam::Hit> h = group_->intersect (r);
	util::Vec3 result;
	if (h->scatter ()) {
		result = h->emission () + (h->albedo () * calculateRadiance (h->scattered_ray (r), depth - 1));
	}
	else {
		result = h->emission ();
	}
	return result;
}
