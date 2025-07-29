#pragma once

#include <spdlog/common.h>

#include <cstddef>
#include <memory>
#include <string>

namespace io
{

void init_spdlog(spdlog::level::level_enum level = spdlog::level::info);

void* aligned_malloc(size_t size, size_t align);
void  aligned_free(void* p);

struct aligned_deleter {
    void operator()(void* p) const { aligned_free(p); }
};

}  // namespace io
