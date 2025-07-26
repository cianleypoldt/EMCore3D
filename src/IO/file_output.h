#pragma once

#include "common/constants.h"
#include "emfdtd/grid/yee_grid.h"
#include <filesystem>
#include <fstream>
#include <spdlog/spdlog.h>
#include <string>

namespace io {

class file_output {
  public:
    enum class entry_type : uint8_t {
        INVALID = 0,
        SINGLE_PRECISION,
        DOUBLE_PRECISION
    };

    file_output() = default;
    ~file_output() { close(); }

    void open(yee_grid* grid,
              const std::string& file_name = "",
              entry_type type = entry_type::SINGLE_PRECISION,
              R compression_factor = 0);

    void close();
    void write();

  private:
    struct file_preface {
        static constexpr uint32_t magic_number = 0xB7A2F91D;
        entry_type type;
        index3 grid_dims;
        vec3 world_size;
    };

    std::ofstream out_stream_;
    std::filesystem::path file_path_;
    yee_grid* grid_ = nullptr;
    entry_type type_ = entry_type::INVALID;
    R compression_factor_ = 0;

    static constexpr uint32_t binary_footer = 0xDEADBEEF;

    void write_header(const yee_grid& grid);
    void write_data(const yee_grid& grid);

    template <typename T>
    void write_field(const T* data, size_t count);

    template <typename T>
    void write_compressed_field(const T* data, size_t count);
};

} // namespace io
