#pragma once

#include "../camera/CamObs.h"
#include "../shape/Background.h"
#include "../shape/Group.h"
#include "Sampler.h"

class Scene : public util::Sampler {
   public:
	Scene(const shapes::Group& group, const shapes::Background& bg,
	      const cam::CamObs& cam, size_t depth);
	util::Vec3 color(float x, float y) const override;
	util::Vec3 calculateRadiance(const cam::Ray& r, size_t depth) const;

   private:
	shapes::Background bg;
	shapes::Group group;
	cam::CamObs cam;
	size_t depth;
};
