#pragma once

#include "yee/grid.hpp"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <string>

namespace io
{

namespace fs
{
[[nodiscard]] bool   file_exists(const std::string& path);
[[nodiscard]] size_t file_size(const std::string& path);
bool                 ensure_directory(const std::string& path);

struct output_file;
[[nodiscard]] output_file start_raw_writer(const YeeGrid* grid, const std::string& file_name, double compression_factor);
void                      write_current_grid_snapshot(output_file of);

static constexpr uint32_t binary_footer = 0xDEADBEEF;

struct output_file {
    const YeeGrid*              grid = nullptr;
    const std::filesystem::path path;
    const bool                  full_float_precision = false;
    const double                compression_factor   = 1;
    std::ofstream               output_stream;
    uint32_t                    frame_count = 0;

    output_file(const output_file&)            = delete;
    output_file& operator=(const output_file&) = delete;
    ~output_file();

  private:
    output_file(const YeeGrid* grid, const std::filesystem::path& path, bool full_float_precision, double compression_factor);

    friend output_file start_f16_writer(const YeeGrid* grid, const std::string& file_name, double compression_factor);
    friend output_file start_raw_writer(const YeeGrid* grid, const std::string& file_name, double compression_factor);
};
}  // namespace fs
}  // namespace io
