#pragma once
#include "../../config/constants.h"
#include <fstream>
#include <spdlog/spdlog.h>

class yee_grid;

namespace io {

void init_spdlog(spdlog::level::level_enum level);
void* aligned_malloc(size_t size, size_t align);
void aligned_free(void* p);

class file_output {

  public:
    file_output(const yee_grid& grid, std::string file_name = "") { open(grid, file_name); };

    ~file_output() { close(); };

    void open(const yee_grid& grid, std::string = "");
    void close();

    void save_raw(const yee_grid& grid, std::string = "");
    void save_single_precision(const yee_grid& grid, std::string = "");
    void save_compressed(const yee_grid& grid, int target_x_resolution, std::string = "");
    void save_compressed_single_precision(const yee_grid& grid, int target_x_resolution, std::string = "");

#pragma pack(push, 1)
    struct file_preface {
        enum class entry_types : unsigned char { SINGLE_PRECISION = 0,
                                                 DOUBLE_PRECISION };
        const uint32_t general_preface_id = 0xB7A2F91D;
        entry_types entry_type;
        index3 grid_dimensions;
        vec3 world_size;
    };
#pragma pack(pop)

  private:
    static constexpr uint32_t binary_border = 0xDEADBEEF;

    std::ofstream ofstream_;
    std::string file_name_;
};
} // namespace io
