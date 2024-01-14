#include "Digit.h"
#include "sRGB.h"
#include <Adafruit_NeoPixel.h>

namespace
{
    void SetSpecificPixels(uint8_t start, uint8_t end, uint32_t color, Adafruit_NeoPixel& strip)
    {
        for (; start <= end; ++start)
        {
            strip.setPixelColor(start, color);
        }
    }

    void SetSpecificPixels(uint8_t* arr, uint8_t size, uint32_t color, Adafruit_NeoPixel& strip)
    {
        for (uint8_t i = 0; i < size; ++i)
        {
            strip.setPixelColor(arr[i], color);
        }
    }
}

Digit::Digit(uint8_t start, uint8_t end, uint8_t* subPixelsArr,
    uint8_t size, Adafruit_NeoPixel& strip)
    : m_start(start)
    , m_end(end)
    , m_subPixelsArrSize(size)
    , m_strip(strip)
{
    if (size > 0 && subPixelsArr)
    {
        m_arrSubPixels = new uint8_t[size];
        for (uint8_t i = 0; i < size; ++i)
        {
            m_arrSubPixels[i] = subPixelsArr[i];
        }
    }
}

Digit::~Digit()
{
    if (m_arrSubPixels)
        delete[] m_arrSubPixels;
}

void Digit::SetColor(const sRGB& value)
{
    const auto color = m_strip.Color(value.red, value.green, value.blue);
    SetSpecificPixels(m_start, m_end, color, m_strip);
}
void Digit::SetSubColor(const sRGB& value)
{
    if (!m_subPixelsArrSize)
        return;
    const auto color = m_strip.Color(value.red, value.green, value.blue);
    SetSpecificPixels(m_arrSubPixels, m_subPixelsArrSize, color, m_strip);
}

void Digit::TurnOff()
{
    const auto color = m_strip.Color(0, 0, 0);
    SetSpecificPixels(m_start, m_end, color, m_strip);
}
