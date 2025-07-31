#pragma once

#include "yee/grid.hpp"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <string>

namespace fs
{
[[nodiscard]] bool   file_exists(const std::string& path);
[[nodiscard]] size_t file_size(const std::string& path);
bool                 ensure_directory(const std::string& path);

namespace binary
{
struct file_preface {
    uint32_t grid_dimensions[3];
    float    cell_size[3];
} __attribute__((packed));

constexpr uint32_t offset_of_frame_count  = 0;
constexpr uint32_t offset_of_preface      = sizeof(uint32_t);
constexpr uint32_t offset_of_permeability = sizeof(uint32_t) + sizeof(file_preface);
constexpr uint32_t size_of_time_stamp     = sizeof(double);

struct output_file;
[[nodiscard]] output_file  initiate_writer(const YeeGrid* grid, const std::string& file_name, double downsampling_factor);
[[nodiscard]] file_preface generate_file_preface(const YeeGrid*) noexcept;
void                       append_current_field(output_file& of);

static constexpr uint32_t padding = 0xDEADBEEF;

struct output_file {
    const std::filesystem::path path;
    std::ofstream               output_stream;
    const file_preface          file_preface;
    const real*                 field_data          = nullptr;
    const double                downsampling_factor = 1;
    uint32_t                    frame_count         = 0;

    output_file(const output_file&)            = delete;
    output_file& operator=(const output_file&) = delete;
    output_file(output_file&&) noexcept        = default;

    ~output_file();
    template <typename T>
    void _write();

  private:
    output_file(const YeeGrid* grid, const std::filesystem::path& path, double downsampling_factor);
    friend output_file initiate_writer(const YeeGrid* grid, const std::string& file_name, double downsampling_factor);
};

}  // namespace binary
}  // namespace fs
