#include "io/file_system.hpp"
#include "yee/grid.hpp"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>

namespace io
{

fs::output_file fs::start_raw_writer(const YeeGrid* grid, const std::string& file_name, double compression_factor) {
    ensure_directory(io_const::bin_output_base_directory);
    try {
        std::filesystem::path path = io_const::bin_output_base_directory + file_name;
        return output_file(grid, path, true, compression_factor);
    } catch (...) {
        spdlog::error("Error occurred while initiating binary output stream.");
        throw std::runtime_error("Could not open output file.");
    }

    //write header
}

fs::output_file fs::start_f16_writer(const YeeGrid* grid, const std::string& file_name, double compression_factor) {
    ensure_directory(io_const::bin_output_base_directory);
    try {
        std::filesystem::path path = io_const::bin_output_base_directory + file_name;
        return output_file(grid, path, false, compression_factor);
    } catch (...) {
        spdlog::error("Error occurred while initiating binary output stream.");
        throw std::runtime_error("Could not open output file.");
    }

    //write header
}

fs::output_file::output_file(const YeeGrid* grid, const std::filesystem::path& path, const bool full_float_precision, const double compression_factor) :
    grid(grid),
    path(path),
    full_float_precision(full_float_precision),
    compression_factor(compression_factor),
    output_stream(path, std::ios::binary) {}

fs::output_file::~output_file() {
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
}  // namespace io
