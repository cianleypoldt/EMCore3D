#pragma once

#include "common/constants.h"
#include <cstddef>
#include <memory>
#include <spdlog/spdlog.h>

namespace io {

/**
 * @brief Initialize spdlog with console and file sinks
 * @param level Minimum log level to display
 */
void init_spdlog(spdlog::level::level_enum level = spdlog::level::info);

/**
 * @brief Allocate aligned memory
 * @param size Size in bytes to allocate
 * @param align Alignment boundary (must be power of 2)
 * @return Pointer to aligned memory, or nullptr on failure
 */
void* aligned_malloc(size_t size, size_t align);

/**
 * @brief Free aligned memory
 * @param p Pointer to memory allocated with aligned_malloc
 */
void aligned_free(void* p);

/**
 * @brief Smart pointer deleter for aligned memory
 */
struct aligned_deleter {
    void operator()(void* p) const {
        aligned_free(p);
    }
};

/**
 * @brief Smart pointer for aligned memory
 */
template <typename T>
using aligned_ptr = std::unique_ptr<T[], aligned_deleter>;

/**
 * @brief Create aligned memory smart pointer
 * @tparam T Type of data
 * @param count Number of elements
 * @param align Alignment boundary
 * @return aligned_ptr<T> Smart pointer to aligned memory
 */
template <typename T>
aligned_ptr<T> make_aligned(size_t count, size_t align = alignof(T)) {
    void* raw_ptr = aligned_malloc(count * sizeof(T), align);
    if (!raw_ptr) {
        throw std::bad_alloc();
    }
    return aligned_ptr<T>(static_cast<T*>(raw_ptr));
}

/**
 * @brief Ensure directory exists
 * @param path Directory path
 * @return true if directory exists or was created successfully
 */
bool ensure_directory(const std::string& path);

/**
 * @brief Get file size
 * @param path File path
 * @return File size in bytes, or 0 if file doesn't exist
 */
size_t file_size(const std::string& path);

/**
 * @brief Check if file exists
 * @param path File path
 * @return true if file exists
 */
bool file_exists(const std::string& path);

} // namespace io
