#include "common/memory_utils.hpp"

#include <spdlog/spdlog.h>

void* io::aligned_malloc(size_t size, size_t align) {
    if (size == 0) {
        return nullptr;
    }

    // Ensure alignment is a power of 2 and >= sizeof(void*)
    if (align < sizeof(void*) || (align & (align - 1)) != 0) {
        spdlog::error("Invalid alignment: {}", align);
        return nullptr;
    }

#ifdef _WIN32
    return _aligned_malloc(size, align);
#else
    void* p = nullptr;
    if (posix_memalign(&p, align, size) != 0) {
        spdlog::error("posix_memalign failed for size {} with alignment {}", size, align);
        p = nullptr;
    }
    return p;
#endif
}

void io::aligned_free(void* p) {
    if (p == nullptr) {
        return;
    }

#ifdef _WIN32
    _aligned_free(p);
#else
    free(p);
#endif
}
