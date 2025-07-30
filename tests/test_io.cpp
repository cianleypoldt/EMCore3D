#include "emfdtd/grid/yee_grid.h"
#include "IO/io.h"
#define CATCH_CONFIG_MAIN

#include "IO/file_sys/file_system.h"

#include <catch2/catch_all.hpp>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <vector>

namespace fs = std::filesystem;

TEST_CASE("ensure_directory creates and verifies directories", "[io][filesystem]") {
    std::string test_dir = "test_output_dir";
    // Remove if exists
    fs::remove_all(test_dir);

    REQUIRE(io::fs::ensure_directory(test_dir));
    REQUIRE(fs::exists(test_dir));
    REQUIRE(fs::is_directory(test_dir));

    // Should not throw or fail if called again
    REQUIRE(io::fs::ensure_directory(test_dir));

    fs::remove_all(test_dir);
}

TEST_CASE("file_exists and file_size work correctly", "[io][filesystem]") {
    std::string       test_file = "test_file_exists.bin";
    std::ofstream     ofs(test_file, std::ios::binary);
    std::vector<char> data(128, 42);
    ofs.write(data.data(), data.size());
    ofs.close();

    REQUIRE(io::fs::file_exists(test_file));
    REQUIRE(io::fs::file_size(test_file) == data.size());

    fs::remove(test_file);
    REQUIRE_FALSE(io::fs::file_exists(test_file));
    REQUIRE(io::fs::file_size(test_file) == 0);
}

TEST_CASE("output_file writes and closes correctly", "[io][output_file]") {
    std::string test_dir = "test_bin_output";
    io::fs::ensure_directory(test_dir);

    std::filesystem::path test_path = test_dir + "/test_output.bin";

    YeeGrid grid({ 3, 3, 3 }, 1);

    {
        auto of = io::fs::start_raw_writer(&grid, test_path, 1.0);
        REQUIRE(of.output_stream.is_open());
        // Write some dummy data
        int dummy = 1234;
        of.output_stream.write(reinterpret_cast<const char*>(&dummy), sizeof(dummy));
    }
    // File should exist and contain the footer
    REQUIRE(fs::exists(test_path));
    std::ifstream ifs(test_path, std::ios::binary | std::ios::ate);
    auto          size = ifs.tellg();
    REQUIRE(size >= sizeof(real) * grid.cell_count * grid.memory_per_cell);
    ifs.seekg(-static_cast<std::streamoff>(sizeof(io::fs::binary_footer)), std::ios::end);
    uint32_t footer = 0;
    ifs.read(reinterpret_cast<char*>(&footer), sizeof(footer));
    REQUIRE(footer == io::fs::binary_footer);

    fs::remove_all(test_dir);
}

TEST_CASE("aligned_malloc and aligned_free work as expected", "[io][aligned_memory]") {
    constexpr size_t size  = 256;
    constexpr size_t align = 64;
    void*            ptr   = io::aligned_malloc(size, align);
    REQUIRE(ptr != nullptr);
    REQUIRE(reinterpret_cast<uintptr_t>(ptr) % align == 0);

    // Write/read to memory
    std::memset(ptr, 0xAB, size);
    auto* bytes = static_cast<unsigned char*>(ptr);
    for (size_t i = 0; i < size; ++i) {
        REQUIRE(bytes[i] == 0xAB);
    }

    io::aligned_free(ptr);
}
