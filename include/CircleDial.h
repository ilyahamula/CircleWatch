#ifndef CIRCLEDIAL_H
#define CIRCLEDIAL_H

#include <Arduino.h>

#define MINUTES 60
#define HOURS 12

class Adafruit_NeoPixel;
class Digit;

struct sRGB
{
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
};

class CircleDial
{
public:
    CircleDial(uint8_t pin);
    ~CircleDial();
    bool SetTime(uint8_t hour, uint8_t min);
    void SetBrightness(uint8_t value);
    void SetHoursColor(sRGB value);
    void SetMinutesColor(sRGB value);
    void Off();

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

#endif
