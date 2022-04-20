#include "Transformation.h"

namespace util {
util::Transformation::Transformation(const Mat4& matrix)
    : to_world(matrix),
      from_world(matrix.invert()),
      to_world_n(matrix.invert().transpose()),
      from_world_n(matrix.invert().invert().transpose()) {
}
}  // namespace util
