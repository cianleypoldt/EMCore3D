#pragma once

#include "common/constants.h"
#include "emfdtd/grid/yee_grid.h"

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>

namespace io
{

class FileOutput
{
  public:
    enum class EntryType : uint8_t { INVALID = 0, SINGLE_PRECISION, DOUBLE_PRECISION };

    FileOutput() = default;

    ~FileOutput() { close(); }

    void open(YeeGrid *           grid,
              const std::string & file_name          = "",
              EntryType           type               = EntryType::SINGLE_PRECISION,
              real                compression_factor = 0);

    void close();
    void write();

  private:
    struct file_preface {
        static constexpr uint32_t magic_number = 0xB7A2F91D;
        EntryType                 type;
        index3                    grid_dims;
        vec3                      world_size;
    };

    std::ofstream         out_stream_;
    std::filesystem::path file_path_;
    YeeGrid *             grid_               = nullptr;
    EntryType             type_               = EntryType::INVALID;
    real                  compression_factor_ = 0;

    static constexpr uint32_t binary_footer = 0xDEADBEEF;

    void write_header(const YeeGrid & grid);
    void write_data(const YeeGrid & grid);

    template <typename T> void write_field(const T * data, size_t count);

    template <typename T> void write_compressed_field(const T * data, size_t count);
};

}  // namespace io
