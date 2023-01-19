#pragma once

#include "Defines.h"

class Adafruit_NeoPixel;

class Digit
{
    uint8_t m_start = 0;
    uint8_t m_end = 0;
    bool m_isOn = false;
    uint8_t m_red = 0;
    uint8_t m_green = 0;
    uint8_t m_blue = 0;
    Adafruit_NeoPixel& m_strip;

public:
    Digit(uint8_t start, uint8_t end, Adafruit_NeoPixel& strip)
        : m_start(start), m_end(end), m_strip(strip) {}
    Digit(uint8_t id, Adafruit_NeoPixel& strip)
        : m_start(id), m_end(id), m_strip(strip) {}
    virtual ~Digit() = default;

    bool IsOn() { return m_isOn; }
    void SetColor(uint8_t r, uint8_t g, uint8_t b)
    {
        m_red = r; m_green = g; m_blue = b;
    }

    void TurnOn();
    void TurnOff();

private:
    void SetSpecificPixels(bool on);
};
