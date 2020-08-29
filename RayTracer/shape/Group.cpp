#include "Group.h"
#include "../material/Material.h"
#include "../tools/Vec3.h"

namespace shapes {
Group::Group(const util::Transformation& transform)
    : shapeList(std::vector<std::shared_ptr<Shape>>())
    , transform(transform)
{
}
Group::Group(const util::Mat4& matrix)
    : shapeList(std::vector<std::shared_ptr<Shape>>())
    , transform(util::Transformation(matrix))
{
}

std::shared_ptr<cam::Hit> Group::intersect(const cam::Ray& r)
{
    cam::Ray imagR(transform.fromWorld.transformPoint(r.x0_), transform.fromWorld.transformDir(r.d_), r.tmin_, r.tmax_, r.normalize_);

    std::shared_ptr<cam::Hit> result;

    for (std::shared_ptr<Shape> s : shapeList) {
        std::shared_ptr<cam::Hit> temp = s->intersect(imagR);
        if (temp != nullptr) {
            if (result == nullptr) {
                result = temp;
            } else if (result->t_ > temp->t_) {
                result = temp;
            }
        }
    }
    if (result != nullptr) {
        result = std::make_shared<cam::Hit>(cam::Hit(transform.toWorld.transformPoint(result->hit_), transform.toWorldN.transformDir(result->n_), result->t_, result->material_));
    }
    return result;
}
void Group::add(const std::shared_ptr<Shape>& shape)
{
    shapeList.push_back(shape);
}
}