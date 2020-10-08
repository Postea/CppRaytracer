#pragma once

#include "Light.h"
#include "Shape.h"

namespace shapes {
class LightShape : public Light, public Shape {};
}  // namespace shapes