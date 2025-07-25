#include "../emfdtd/grid/yee_grid.h"
#include "io.h"

namespace io {

void file_output::open(const yee_grid& grid, std::string file_name) {
    if (file_name == file_name_) return;

    close();

    file_name_ = file_name.empty() ? "unnamed_grid_snapshot.bin" : file_name;
    ofstream_.open(file_name_, std::ios::binary);

    if (!ofstream_.is_open()) {
        throw std::runtime_error("failed to open output file: " + file_name_);
    }

    file_preface fp;
    fp.entry_type = file_preface::entry_types::SINGLE_PRECISION;
    fp.grid_dimensions = grid.grid_dimensions;
    fp.world_size = grid.world_size;

    ofstream_.write(reinterpret_cast<const char*>(&fp), sizeof(file_preface));
}

void file_output::close() {
    if (ofstream_.is_open()) {
        ofstream_.flush();
        ofstream_.close();
    }
}

void file_output::save_raw(const yee_grid& grid, std::string file_name) {
    if (!ofstream_.is_open() || !file_name.empty()) {
        open(grid, file_name);
    }

    // update entry type in header
    ofstream_.seekp(offsetof(file_preface, entry_type), std::ios::beg);
    file_preface::entry_types type =
        std::is_same<R, float>::value ? file_preface::entry_types::SINGLE_PRECISION : file_preface::entry_types::DOUBLE_PRECISION;
    ofstream_.write(reinterpret_cast<const char*>(&type), sizeof(type));

    // append raw grid data
    ofstream_.seekp(0, std::ios::end);
    ofstream_.write(reinterpret_cast<const char*>(grid.grid_allocation), grid.cell_count * yee_grid::memory_per_cell);
}

void file_output::save_single_precision(const yee_grid& grid, std::string file_name) {
    if (!ofstream_.is_open() || !file_name.empty()) {
        open(grid, file_name);
    }

    // update entry type in header
    file_preface::entry_types type =
        std::is_same<R, float>::value ? file_preface::entry_types::SINGLE_PRECISION : file_preface::entry_types::DOUBLE_PRECISION;

    ofstream_.seekp(offsetof(file_preface, entry_type), std::ios::beg);
    ofstream_.write(reinterpret_cast<const char*>(&type), sizeof(type));

    ofstream_.seekp(0, std::ios::end);

    if constexpr (std::is_same<R, float>::value) {
        // direct write, already float
        ofstream_.write(reinterpret_cast<const char*>(grid.grid_allocation),
                        grid.cell_count * yee_grid::memory_per_cell);
        return;
    }

    // convert to float per entry
    const R* data = reinterpret_cast<const R*>(grid.grid_allocation);
    const std::size_t total_elements = grid.cell_count * yee_grid::memory_per_cell;

    for (std::size_t i = 0; i < total_elements; ++i) {
        float value = static_cast<float>(data[i]);
        ofstream_.write(reinterpret_cast<const char*>(&value), sizeof(float));
    }
}

void file_output::save_compressed(const yee_grid& grid, int target_x_resolution, std::string) {
}

void file_output::save_compressed_single_precision(const yee_grid& grid, int target_x_resolution, std::string) {
}

} // namespace io
