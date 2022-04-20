#include "LightSingleGroup.h"

#include "../material/Material.h"
#include "../tools/Vec3.h"

namespace shapes {
LightSingleGroup::LightSingleGroup(const util::Transformation& transform,
                                   std::shared_ptr<Light> light)
    : light(light), transform(transform) {
}
LightSingleGroup::LightSingleGroup(const util::Mat4& matrix,
                                   std::shared_ptr<Light> light)
    : light(light), transform(util::Transformation(matrix)) {
}

util::SurfacePoint LightSingleGroup::sample_light(const cam::Hit& h) const {
	cam::Hit xx(transform.from_world.transform_point(h.point()),
	            transform.from_world_n.transform_direction(h.normal()),
	            h.texel(), h.scalar(), h.material);
	auto sample = light->sample_light(xx);
	return util::SurfacePoint(
	    transform.to_world.transform_point(sample.point()),
	    transform.to_world_n.transform_direction(sample.normal()),
	    sample.texel(), sample.material);
}

util::Vec3 LightSingleGroup::light_emission(const util::SurfacePoint& p) const {
	return light->light_emission(p);
}

float LightSingleGroup::light_pdf(const util::SurfacePoint& p,
                                  const util::Vec3& dl_out) const {
	return light->light_pdf(p, dl_out);
}

}  // namespace shapes