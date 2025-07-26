#include "file_output.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace io {

void file_output::open(yee_grid* grid_ptr,
                       const std::string& file_name,
                       entry_type type,
                       R compression_factor) {
    if (!grid_ptr) {
        spdlog::error("Cannot open file output: grid pointer is null");
        throw std::invalid_argument("Grid pointer must not be null.");
    }

    close();

    grid_ = grid_ptr;
    type_ = type;
    compression_factor_ = compression_factor;

    fs::path base_path = "../results";
    fs::create_directories(base_path); // Ensure directory exists

    file_path_ = file_name.empty()
                     ? base_path / "unnamed_grid_snapshot.bin"
                     : base_path / file_name;

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

void file_output::close() {
    if (out_stream_.is_open()) {
        try {
            out_stream_.write(reinterpret_cast<const char*>(&binary_footer), sizeof(binary_footer));
            out_stream_.flush();
        } catch (...) {
            spdlog::warn("Error occurred while writing footer.");
        }
        out_stream_.close();
    }
}

void file_output::write() {
    if (!grid_) {
        spdlog::error("Cannot write file: grid pointer is null");
        return;
    }
    write_data(*grid_);
}

void file_output::write_header(const yee_grid& grid) {
    file_preface header{};
    header.type = type_;
    header.grid_dims = grid.grid_dimensions;
    header.world_size = grid.world_size;

    out_stream_.write(reinterpret_cast<const char*>(&header), sizeof(header));
}

void file_output::write_data(const yee_grid& grid) {
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
        write_all([this](const auto* data, size_t n) {
            write_compressed_field(data, n);
        });
    } else if (type_ == entry_type::SINGLE_PRECISION && !std::is_same_v<R, float>) {
        write_all([this](const auto* data, size_t n) {
            std::vector<float> tmp(n);
            for (size_t i = 0; i < n; ++i)
                tmp[i] = static_cast<float>(data[i]);
            write_field(tmp.data(), n);
        });
    } else {
        write_all([this](const auto* data, size_t n) {
            write_field(data, n);
        });
    }
}

template <typename T>
void file_output::write_field(const T* data, size_t count) {
    out_stream_.write(reinterpret_cast<const char*>(data), count * sizeof(T));
}

template <typename T>
void file_output::write_compressed_field(const T* data, size_t count) {
    size_t stride = static_cast<size_t>(compression_factor_);
    if (stride < 1) stride = 1;

    for (size_t i = 0; i < count; i += stride) {
        out_stream_.write(reinterpret_cast<const char*>(&data[i]), sizeof(T));
    }
}

} // namespace io
