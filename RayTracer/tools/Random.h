#pragma once

#include <random>

#include "Vec3.h"

namespace util {

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_real_distribution<> dis0to1(0.0, 1.0);
static std::uniform_real_distribution<> disMinus1To1(-1.0, 1.0);
Vec3 rand_vec3();
bool in_circle(const Vec3& v);
Vec3 rand_vec3_in_circle(float r);
}  // namespace util
