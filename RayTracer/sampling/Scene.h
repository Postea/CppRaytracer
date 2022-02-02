#pragma once

#include <vector>

#include "../camera/CamObs.h"
#include "../shape/Group.h"
#include "../shape/Light.h"
#include "OptiSampler.h"

class Scene : public util::OptiSampler {
   public:
	Scene(const shapes::Group& group,
	      std::vector<std::shared_ptr<shapes::Light>> lights,
	      const cam::CamObs& cam, size_t max_depth, size_t light_n);
	std::pair<util::Vec3, std::vector<int64_t>> color_opti(
	    float x, float y) const override;
	std::pair<util::Vec3, std::vector<int64_t>> calculateRadiance(
	    const cam::Ray& r, size_t depth) const;

	cam::CamObs cam;

   private:
	std::pair<util::Vec3, int64_t> directLighting(
	    const std::optional<cam::Hit>& h, cam::Ray r, int n) const;
	shapes::Group group;
	std::vector<std::shared_ptr<shapes::Light>> lights;
	size_t max_depth;
	size_t light_n;
	bool direct;
};
