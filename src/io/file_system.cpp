#include "io/file_system.hpp"

#include "spdlog/spdlog.h"

bool io::fs::ensure_directory(const std::string& path) {
    try {
        return std::filesystem::create_directories(path);
    } catch (const std::exception& e) {
        spdlog::error("Failed to create directory '{}': {}", path, e.what());
        return false;
    }
}

size_t io::fs::file_size(const std::string& path) {
    try {
        if (std::filesystem::exists(path)) {
            return std::filesystem::file_size(path);
        }
    } catch (const std::exception& e) {
        spdlog::error("Failed to get file size for '{}': {}", path, e.what());
    }
    return 0;
}

bool io::fs::file_exists(const std::string& path) {
    try {
        return std::filesystem::exists(path);
    } catch (const std::exception& e) {
        spdlog::error("Failed to check file existence for '{}': {}", path, e.what());
        return false;
    }
}
