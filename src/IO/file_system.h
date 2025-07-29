#pragma once

#include "common/constants.h"
#include "emfdtd/grid/yee_grid.h"

#include <spdlog/spdlog.h>

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>

namespace io
{

namespace fs
{

bool ensure_directory(const std::string& path);
bool file_exists(const std::string& path);

size_t file_size(const std::string& path);

struct output_file;
output_file start_f16_writer(const YeeGrid* grid, const std::filesystem::path& file_path, double compression_factor);
output_file start_raw_writer(const YeeGrid* grid, const std::filesystem::path& file_path, double compression_factor);

void save_frame(output_file of);

static constexpr uint32_t binary_footer = 0xDEADBEEF;

struct output_file {
    const YeeGrid*              grid = nullptr;
    const std::filesystem::path path;
    const bool                  full_float_precision = false;
    const double                compression_factor   = 1;
    std::ofstream               output_stream;

    output_file(const output_file&)            = delete;
    output_file& operator=(const output_file&) = delete;
    ~output_file();

  private:
    output_file(const YeeGrid*               grid,
                const std::filesystem::path& path,
                const bool                   full_float_precision,
                const double                 compression_factor);

    friend output_file start_f16_writer(YeeGrid*                     grid,
                                        const std::filesystem::path& file_path,
                                        double                       compression_factor);
    friend output_file start_raw_writer(YeeGrid*                     grid,
                                        const std::filesystem::path& file_path,
                                        double                       compression_factor);
};
}  // namespace fs
}  // namespace io
