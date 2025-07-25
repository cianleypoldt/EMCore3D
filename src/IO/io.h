#pragma once
#include "../../config/constants.h"
#include <ostream>
#include <spdlog/spdlog.h>

class yee_grid;

namespace io {

void init_spdlog(spdlog::level::level_enum level);

class file_output {

    void save_raw(const yee_grid& grid);
    void save_single_precision(const yee_grid& grid);
    void save_compressed(const yee_grid& grid, uint compression_factor);
    void save_compressed_single_precision(const yee_grid& grid, uint compression_factor);

  private:
};
}; // namespace io
