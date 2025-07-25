#include "../emfdtd/grid/yee_grid.h"
#include "io.h"

void io::file_output::open(const yee_grid& grid, std::string_view file_name) {
    if (file_name == file_name_) return;

    close();

    if (file_name == "") {
        ofstream_.open("unnamed_grid_snapshot.bin", std::ios::binary);
    } else {
        ofstream_.open(file_name.data(), std::ios::binary);
    }

    ofstream_ << grid.GridDimensions[0] << grid.GridDimensions[1] << grid.GridDimensions[2] << binary_border;
}

void io::file_output::save_raw(const yee_grid& grid, std::string_view file_name) {

    if (!ofstream_.is_open() || file_name != "") open(grid, file_name);
}

void io::file_output::save_single_precision(const yee_grid& grid, std::string_view) {}
void io::file_output::save_compressed(const yee_grid& grid, R compression_factor, std::string_view) {}
void io::file_output::save_compressed_single_precision(const yee_grid& grid, R compression_factor, std::string_view) {}
