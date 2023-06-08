#pragma once

#include "Defines.h"

class sRGB;
class Adafruit_NeoPixel;

class Digit
{
    uint8_t m_start = 0;
    uint8_t m_end = 0;
    Adafruit_NeoPixel& m_strip;
    uint8_t* m_arrSubPixels = nullptr;
    uint8_t m_subPixelsArrSize = 0;

public:
    Digit(uint8_t start, uint8_t end, uint8_t* subPixelsArr,
        uint8_t size, Adafruit_NeoPixel& strip);
    Digit(uint8_t id, Adafruit_NeoPixel& strip)
        : m_start(id), m_end(id), m_strip(strip) {}
    ~Digit();

    void SetColor(const sRGB& value);
    void SetSubColor(const sRGB& value);

    void TurnOff();
};
