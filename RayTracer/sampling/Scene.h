#pragma once

#include "../camera/CamObs.h"
#include "../camera/Ray.h"
#include "../shape/Group.h"
#include "../tools/Vec3.h"
#include "Sampler.h"

class Scene : public util::Sampler {
   public:
	Scene(std::shared_ptr<shapes::Shape> group, cam::CamObs cam, size_t depth);
	util::Vec3 color(float x, float y) const override;
	util::Vec3 calculateRadiance(const cam::Ray& r, size_t depth) const;

   private:
	std::shared_ptr<shapes::Shape> group;
	cam::CamObs cam;
	size_t depth;
};
