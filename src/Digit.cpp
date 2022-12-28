#include "Digit.h"

#include <Adafruit_NeoPixel.h>

void Digit::TurnOn()
{
    SetSpecificPixels(true);
    m_isOn = true;
}

void Digit::TurnOff()
{
    SetSpecificPixels(false);
    m_isOn = false;
}

void Digit::SetSpecificPixels(bool on)
{
    uint8_t r = (on ? m_red : 0);
    uint8_t g = (on ? m_green : 0);
    uint8_t b = (on ? m_blue : 0);
    const auto color = m_strip.Color(r, g, b);

    unsigned i = m_start;
    do
    {
        m_strip.setPixelColor(i++, color);
    }
    while (i <= m_end);      
    //m_strip.show();
}
