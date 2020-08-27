
#include "Sampler.h"
#include "Vec3.h"
#include "Ray.h"
#include "CamObs.h"
#include "Group.h"

class Scene : public util::Sampler
{
public:
	Scene (std::shared_ptr<shapes::Shape> group, cam::CamObs cam, size_t depth);
	util::Vec3 color (float x, float y) const;
	util::Vec3 calculateRadiance (const cam::Ray& r, size_t depth) const;
private:
	std::shared_ptr<shapes::Shape> group_;
	cam::CamObs cam_;
	size_t depth_;
};
