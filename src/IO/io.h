#pragma once
#include "../../config/constants.h"
#include <fstream>
#include <spdlog/spdlog.h>

class yee_grid;

namespace io {

void init_spdlog(spdlog::level::level_enum level);

class file_output {

  public:
    file_output(const yee_grid& grid, std::string_view file_name = "") { open(grid, file_name); };

    ~file_output() { close(); };

    void open(const yee_grid& grid, std::string_view = "");
    void close();

    void save_raw(const yee_grid& grid, std::string_view = "");
    void save_single_precision(const yee_grid& grid, std::string_view = "");
    void save_compressed(const yee_grid& grid, R compression_factor, std::string_view = "");
    void save_compressed_single_precision(const yee_grid& grid, R compression_factor, std::string_view = "");

  private:
    static constexpr unsigned char binary_border = 0b11111111;

    std::ofstream ofstream_;
    std::string_view file_name_;
};
}; // namespace io
