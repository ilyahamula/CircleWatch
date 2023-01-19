#pragma once

using uint8_t = unsigned char;

struct sRGB
{
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    sRGB(uint8_t r, uint8_t g, uint8_t b) : red(r), green(g), blue(b) {};
    sRGB() = default;
};
