#pragma once

#include <vector>

#include "../camera/CamObs.h"
#include "../shape/Group.h"
#include "../shape/Light.h"
#include "Sampler.h"

class Scene : public util::Sampler {
   public:
	Scene(const shapes::Group& group,
	      std::vector<std::shared_ptr<shapes::Light>> lights,
	      const cam::CamObs& cam, size_t depth);
	util::Vec3 color(float x, float y) const override;
	util::Vec3 calculateRadiance(const cam::Ray& r, size_t depth) const;

	cam::CamObs cam;

   private:
	util::Vec3 directLighting(const std::optional<cam::Hit>& h, cam::Ray r,
	                          int nn) const;
	shapes::Group group;
	std::vector<std::shared_ptr<shapes::Light>> lights;
	size_t depth;
};
