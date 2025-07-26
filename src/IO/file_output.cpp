#include "file_output.h"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

namespace io
{

void FileOutput::open(YeeGrid * grid_ptr, const std::string & file_name, EntryType type, real compression_factor) {
    if (grid_ptr == nullptr) {
        spdlog::error("Cannot open file output: grid pointer is null");
        throw std::invalid_argument("Grid pointer must not be null.");
    }

    close();

    grid_               = grid_ptr;
    type_               = type;
    compression_factor_ = compression_factor;

    fs::path base_path = "../results";
    fs::create_directories(base_path);  // Ensure directory exists

    file_path_ = file_name.empty() ? base_path / "unnamed_grid_snapshot.bin" : base_path / file_name;

    // Check if file exists and remove it if it does
    if (fs::exists(file_path_)) {
        fs::remove(file_path_);
    }

    out_stream_.open(file_path_, std::ios::binary | std::ios::out);
    if (!out_stream_.is_open()) {
        spdlog::error("Failed to open output file: {}", file_path_.string());
        throw std::runtime_error("Could not open output file.");
    }

    write_header(*grid_);
}

void FileOutput::close() {
    if (out_stream_.is_open()) {
        try {
            out_stream_.write(reinterpret_cast<const char *>(&binary_footer), sizeof(binary_footer));
            out_stream_.flush();
        } catch (...) {
            spdlog::warn("Error occurred while writing footer.");
        }
        out_stream_.close();
    }
}

void FileOutput::write() {
    if (grid_ == nullptr) {
        spdlog::error("Cannot write file: grid pointer is null");
        return;
    }
    write_data(*grid_);
}

void FileOutput::write_header(const YeeGrid & grid) {
    file_preface header{};
    header.type       = type_;
    header.grid_dims  = grid.grid_dimensions;
    header.world_size = grid.world_size;

    out_stream_.write(reinterpret_cast<const char *>(&header), sizeof(header));
}

void FileOutput::write_data(const YeeGrid & grid) {
    const size_t count = grid.cell_count;

    auto write_all = [&](auto writer) {
        writer(grid.m_Ex, count);
        writer(grid.m_Ey, count);
        writer(grid.m_Ez, count);
        writer(grid.m_Hx, count);
        writer(grid.m_Hy, count);
        writer(grid.m_Hz, count);
        writer(grid.inv_permittivity, count);
        writer(grid.inv_permeability, count);
    };

    if (compression_factor_ > 1e-6) {
        write_all([this](const auto * data, size_t n) { write_compressed_field(data, n); });
    } else if (type_ == EntryType::SINGLE_PRECISION && !std::is_same_v<real, float>) {
        write_all([this](const auto * data, size_t n) {
            std::vector<float> tmp(n);
            for (size_t i = 0; i < n; ++i) {
                tmp[i] = static_cast<float>(data[i]);
            }
            write_field(tmp.data(), n);
        });
    } else {
        write_all([this](const auto * data, size_t n) { write_field(data, n); });
    }
}

template <typename T> void FileOutput::write_field(const T * data, size_t count) {
    out_stream_.write(reinterpret_cast<const char *>(data), count * sizeof(T));
}

template <typename T> void FileOutput::write_compressed_field(const T * data, size_t count) {
    auto stride = static_cast<size_t>(compression_factor_);
    stride      = std::max<size_t>(stride, 1);

    for (size_t i = 0; i < count; i += stride) {
        out_stream_.write(reinterpret_cast<const char *>(&data[i]), sizeof(T));
    }
}

}  // namespace io
