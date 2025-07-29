#include "common/constants.h"
#include "emfdtd/grid/yee_grid.h"
#include "file_system.h"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>

namespace io
{

fs::output_file fs::start_raw_writer(YeeGrid* grid, const std::filesystem::path& file_path, double compression_factor) {
    ensure_directory(io_const::bin_output_base_directory);
    try {
        return output_file(grid, file_path, true, compression_factor);
    } catch (...) {
        spdlog::error("Error occurred while initiating binary output stream.");
        throw std::runtime_error("Could not open output file.");
    }

    //write header
}

fs::output_file fs::start_f16_writer(YeeGrid* grid, const std::filesystem::path& file_path, double compression_factor) {
    ensure_directory(io_const::bin_output_base_directory);
    try {
        return output_file(grid, file_path, false, compression_factor);
    } catch (...) {
        spdlog::error("Error occurred while initiating binary output stream.");
        throw std::runtime_error("Could not open output file.");
    }

    //write header
}

}  // namespace io

namespace io::fs
{
output_file::output_file(const YeeGrid*               grid,
                         const std::filesystem::path& path,
                         const bool                   full_float_precision,
                         const double                 compression_factor) :
    grid(grid),
    path(path),
    full_float_precision(full_float_precision),
    compression_factor(compression_factor),
    output_stream(path, std::ios::binary) {}

output_file::~output_file() {
    if (output_stream.is_open()) {
        try {
            output_stream.write(reinterpret_cast<const char*>(&binary_footer), sizeof(binary_footer));
            output_stream.flush();
        } catch (...) {
            spdlog::warn("Error occurred while writing footer.");
        }
        output_stream.close();
    }
}
}  // namespace io::fs
