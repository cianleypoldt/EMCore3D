#include "io/file_system.hpp"
#include "yee/grid.hpp"

#include <spdlog/spdlog.h>

#include <exception>
#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>

namespace fs::binary
{

output_file initiate_writer(const YeeGrid* grid, const std::string& file_name, double downsampling_factor) {
    ensure_directory(io_const::time_series_output_dir);
    try {
        std::filesystem::path path = std::filesystem::path(io_const::time_series_output_dir) / file_name;

        output_file of(grid, path, downsampling_factor);
        if (!of.output_stream.is_open()) {
            spdlog::error("Output stream is not open for file {}", path.string());
            throw std::runtime_error("Output stream is not open.");
        }

        // write file preface generated from grid data at file beginning
        of.output_stream.seekp(offset_of_preface, std::ios::beg);
        of.output_stream.write(reinterpret_cast<const char*>(&of.file_preface), sizeof(of.file_preface));

        // write constant grid components permeability and permittivity
        of.output_stream.seekp(offset_of_permeability, std::ios::beg);
        for (int i = 0; i < grid->cell_count; i++) {
            real cell_permeability = 1 / grid->inv_permeability[i];
            of.output_stream.write(reinterpret_cast<const char*>(&cell_permeability), sizeof(cell_permeability));
        }
        for (int i = 0; i < grid->cell_count; i++) {
            real cell_permittivity = 1 / grid->inv_permittivity[i];
            of.output_stream.write(reinterpret_cast<const char*>(&cell_permittivity), sizeof(cell_permittivity));
        }
        if (!of.output_stream) {
            spdlog::error("Failed to write padding to {}", path.string());
            throw std::runtime_error("Failed to write padding.");
        }

        return of;
    } catch (const std::exception& e) {
        spdlog::error("Error occurred while initiating binary output stream. {}", e.what());
        throw std::runtime_error("Could not open output file.");
    }
}

file_preface generate_file_preface(const YeeGrid* grid) noexcept {
    return file_preface{
        { (uint32_t)grid->grid_dimensions[0], (uint32_t)grid->grid_dimensions[1], (uint32_t)grid->grid_dimensions[2] },
        { (float)grid->cell_dimensions[0],    (float)grid->cell_dimensions[1],    (float)grid->cell_dimensions[2]    }
    };
}

void append_current_field(output_file& of, double time_step) {
    of.output_stream.seekp(0, std::ios::end);
    of.write(time_step) if (of.downsampling_factor == 1) {
        of.output_stream.write(reinterpret_cast<const char*>(of.field_data), YeeGrid::memory_per_cell * of.file_preface.grid_dimensions[0] * of.file_preface.grid_dimensions[1] * of.file_preface.grid_dimensions[2]);
    }
}

output_file::output_file(const YeeGrid* grid, const std::filesystem::path& path, double downsampling_factor) :
    path(path),
    file_preface(generate_file_preface(grid)),
    field_data(grid ? (real*)grid->grid_allocation : nullptr),
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

output_file::~output_file() {
    if (output_stream.is_open()) {
        output_stream.close();
    }
}
}  // namespace fs::binary
