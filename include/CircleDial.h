#pragma once

#include "Defines.h"
#include "sRGB.h"

class Adafruit_NeoPixel;
class Digit;

class CircleDial
{
public:
    CircleDial(uint8_t pin);
    ~CircleDial();
    bool SetTime(uint8_t hour, uint8_t min);
    void SetBrightness(uint8_t value);
    void SetHoursColor(const sRGB& value);
    void SetMinutesColor(const sRGB& value);
    void Show();

private:
    Adafruit_NeoPixel* m_strip;

private:
    Digit* m_digits[MINUTES];
    Digit* m_hoursDigits[HOURS];
    Digit* m_currHourDigit;
    Digit* m_currMinDigit;

    sRGB m_hourColor;
    sRGB m_minuteColor;
};
