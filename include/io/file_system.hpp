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

struct file_preface {
    uint32_t frame_count;
    uint32_t grid_dimensions[3];
    float    cell_size[3];
} __attribute__((packed));

struct output_file;

[[nodiscard]] output_file initiate_writer(const YeeGrid* grid, const std::string& file_name, double downsampling_factor);
file_preface              generate_file_preface(const YeeGrid*, uint32_t frame_count);
void                      append_current_field(output_file& of);

static constexpr uint32_t padding = 0xDEADBEEF;

struct output_file {
    const YeeGrid*        grid = nullptr;
    std::filesystem::path path;
    double                downsampling_factor = 1;
    std::ofstream         output_stream;
    uint32_t              frame_count = 0;

    output_file(const output_file&)            = delete;
    output_file& operator=(const output_file&) = delete;
    output_file(output_file&&) noexcept        = default;

    output_file& operator=(output_file&& other) noexcept {
        if (this != &other) {
            grid                = other.grid;
            path                = other.path;
            downsampling_factor = other.downsampling_factor;
            output_stream       = std::move(other.output_stream);
            frame_count         = other.frame_count;
        }
        return *this;
    }

    ~output_file();

  private:
    output_file(const YeeGrid* grid, const std::filesystem::path& path, double downsampling_factor);
    friend output_file initiate_writer(const YeeGrid* grid, const std::string& file_name, double downsampling_factor);
};

}  // namespace fs
}  // namespace io
