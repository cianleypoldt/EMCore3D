#pragma once

#include "common/types.hpp"

namespace em_const
{
constexpr uint memory_cap        = 1000000000;
constexpr real default_cube_size = 1;
constexpr real default_time_step = 0.001;
}  // namespace em_const

namespace io_const
{
constexpr const char* time_series_output_dir = "/resources/binary_saves/";
}
