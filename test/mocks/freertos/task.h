#pragma once

#include "freertos/FreeRTOS.h"

inline TickType_t mock_tick_count = 0;

inline TickType_t xTaskGetTickCount()
{
    return mock_tick_count;
}

inline void vTaskDelay(TickType_t ticks)
{
    
}
