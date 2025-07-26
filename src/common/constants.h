#pragma once

#include <sys/types.h>

#include <eigen3/Eigen/Dense>

using vec3   = Eigen::Vector3d;
using index3 = Eigen::Vector3i;
using real   = double;

namespace em_const
{
constexpr uint memory_cap        = 1000000000;
constexpr real default_cube_size = 1;
constexpr real default_time_step = 0.001;
}  // namespace em_const
