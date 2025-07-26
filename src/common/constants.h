#pragma once

#include <eigen3/Eigen/Dense>

using vec3 = Eigen::Vector3d;
using index3 = Eigen::Vector3i;
using R = double;

namespace em_const {
constexpr uint memory_cap = 1000000000;
constexpr R default_cube_size = 1;
constexpr R default_time_step = 0.001;
} // namespace em_const
