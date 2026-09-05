#pragma once

#include <stdint.h>

typedef struct
{
    int cores;
    int revision;
} esp_chip_info_t;

inline void esp_chip_info(esp_chip_info_t* info)
{
    if (info)
    {
        info->cores = 2;
        info->revision = 1;
    }
}
