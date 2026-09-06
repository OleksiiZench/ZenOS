#pragma once

#include <stdlib.h>
#include <stdint.h>

#define MALLOC_CAP_DMA 1

inline void* heap_caps_malloc(size_t size, uint32_t caps)
{
    return malloc(size);
}
