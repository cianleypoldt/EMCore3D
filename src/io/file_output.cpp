#include "io/file_system.hpp"
#include "yee/grid.hpp"

#include <spdlog/spdlog.h>

#include <exception>
#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace io
{

fs::output_file fs::initiate_writer(const YeeGrid* grid, const std::string& file_name, double downsampling_factor) {
    ensure_directory(io_const::time_series_output_dir);
    try {
        std::filesystem::path path = io_const::time_series_output_dir + file_name;

        // write file preface generated from grid data at file beginning
        output_file  of(grid, path, downsampling_factor);
        file_preface fp = generate_file_preface(grid, 0);
        of.output_stream.write(reinterpret_cast<const char*>(&fp), sizeof(fp));

        // write constant gird components permeability and permittivity
        of.output_stream.write(reinterpret_cast<const char*>(grid->inv_permeability), grid->cell_count * sizeof(real));
        of.output_stream.write(reinterpret_cast<const char*>(grid->inv_permittivity), grid->cell_count * sizeof(real));

        //follow with verification
        of.output_stream.write(reinterpret_cast<const char*>(&padding), sizeof(padding));

        return of;
    } catch (const std::exception& e) {
        spdlog::error("Error occurred while initiating binary output stream. {}", e.what());
        throw std::runtime_error("Could not open output file.");
    }

    //write header
}

fs::file_preface fs::generate_file_preface(const YeeGrid* grid, uint32_t frame_count) {
    return file_preface{
        frame_count,
        { (uint32_t)grid->grid_dimensions[0], (uint32_t)grid->grid_dimensions[1], (uint32_t)grid->grid_dimensions[2] },
        { (float)grid->cell_dimensions[0],    (float)grid->cell_dimensions[1],    (float)grid->cell_dimensions[2]    }
    };
}

void fs::append_current_field(output_file& of) {
}

fs::output_file::output_file(const YeeGrid* grid, const std::filesystem::path& path, double downsampling_factor) :
    grid(grid),
    path(path),
    downsampling_factor(downsampling_factor) {
    try {
        output_stream.open(path, std::ios::binary);
        if (!output_stream.is_open()) {
            spdlog::error("Failed to open output file {}", path.string());
            throw std::runtime_error("Output file stream not open.");
        }
    } catch (const std::exception& e) {
        spdlog::error("Failed to open output file {}. {}", path.string(), e.what());
        throw std::runtime_error("Output file stream not open");
    }
}

fs::output_file::~output_file() {
    if (output_stream.is_open()) {
        output_stream.close();
    }
}
}  // namespace io
