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
	util::Vec3 calculateRadiance(const cam::Ray& r, size_t depth,
	                             bool& write) const;

   private:
	util::Vec3 directLighting(const std::optional<cam::Hit>& h, cam::Ray r,
	                          int nn) const;
	float Scene::bsdfMisWeight(const std::optional<cam::Hit>& h,
	                           const cam::Ray& scatterRay,
	                           size_t lights_samples) const;
	float Scene::lightMisWeight(std::shared_ptr<shapes::Light> light,
	                            const std::optional<cam::Hit>& h,
	                            const util::Vec3& d_in,
	                            const util::SurfacePoint& samplePoint,
	                            size_t light_samples) const;
	shapes::Group group;
	std::vector<std::shared_ptr<shapes::Light>> lights;
	cam::CamObs cam;
	size_t depth;
};
