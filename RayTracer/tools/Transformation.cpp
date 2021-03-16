#include "Transformation.h"

namespace util {
util::Transformation::Transformation(const Mat4& matrix)
    : toWorld(matrix),
      fromWorld(matrix.invertFull()),
      toWorldN(matrix.invertFull().transpose()),
      fromWorldN(matrix.invertFull().invertFull().transpose()) {
}
}  // namespace util
