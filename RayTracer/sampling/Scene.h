#pragma once

#include <vector>

#include "../camera/CamObs.h"
#include "../shape/Group.h"
#include "../shape/Light.h"
#include "Sampler.h"

class Scene : public util::Sampler {
   public:
	Scene(const shapes::Group& group,
	      const std::vector<std::shared_ptr<shapes::Light>>& lights,
	      const cam::CamObs& cam, size_t max_depth, size_t light_n);

	util::Vec3 color(float x, float y) const override;
	util::Vec3 calculate_radiance(const cam::Ray& r, size_t depth) const;

	const cam::CamObs cam;

   private:
	util::Vec3 direct_lighting(const cam::Hit& h, const cam::Ray& r) const;
	shapes::Group group;
	std::vector<std::shared_ptr<shapes::Light>> lights;
	size_t max_depth;
	size_t light_n;
	bool direct;
};
