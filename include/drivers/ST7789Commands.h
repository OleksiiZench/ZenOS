#pragma once

#include <cstdint>

// Command set for the ST7789 display controller.
namespace ST7789Cmd
{
    constexpr uint8_t SWRESET = 0x01; // Software reset
    constexpr uint8_t SLPOUT  = 0x11; // Sleep out
    constexpr uint8_t COLMOD  = 0x3A; // Interface pixel format
    constexpr uint8_t MADCTL  = 0x36; // Memory data access control
    constexpr uint8_t INVON   = 0x21; // Display inversion on
    constexpr uint8_t DISPON  = 0x29; // Display on
    constexpr uint8_t CASET   = 0x2A; // Column address set
    constexpr uint8_t RASET   = 0x2B; // Row address set
    constexpr uint8_t RAMWR   = 0x2C; // Memory write
}
