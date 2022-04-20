#pragma once

#include <fstream>
#include <vector>

#include "../shape/Triangle.h"

namespace util {
std::vector<shapes::Triangle> load_obj(
    std::istream& in,
    const std::shared_ptr<material::Material>& material = nullptr);
}  // namespace util