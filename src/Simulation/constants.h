#pragma once
#include <eigen3/Eigen/Dense>

using vec3 = Eigen::Vector3d;
using index3 = uint32_t[3];
using uint = uint32_t;
using R = double;

namespace em_const {
constexpr uint MEMORY_CAP = 1000000000;
constexpr R DEFAULT_CUBE_SIZE = 1;
constexpr R DEFAULT_TIME_STEP = 0.001;
} // namespace em_const
