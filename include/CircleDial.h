#pragma once

#include "Defines.h"
#include "sRGB.h"

class Adafruit_NeoPixel;
class Digit;
class Segment;

enum class eDialMode : uint8_t
{
    DigitHour,
    SegmentHour
};

class CircleDial
{
public:
    CircleDial(uint8_t pin);
    ~CircleDial();
    bool SetTime(uint8_t hour, uint8_t min);
    void SetBrightness(uint8_t value);
    void SetHoursColor(const sRGB& value);
    void SetMinutesColor(const sRGB& value);
    void SetMode(eDialMode mode);
    void Show();
    void TurnOff();

private:
    void SetDigitHourTime(uint8_t hour, uint8_t min);
    void SetSegmentHourTime(uint8_t hour, uint8_t min);
    void OffCurrent();

private:
    int8_t m_currHour;
    int8_t m_currMin;
    eDialMode m_mode;
    bool m_forceUpdate;

    sRGB m_hourColor;
    sRGB m_minuteColor;

    Segment* m_segments[HOURS];
    Segment* m_currSeg;
    Digit* m_digits[MINUTES];
    Digit* m_hoursDigits[HOURS];
    Digit* m_currHourDigit;
    Digit* m_currMinDigit;

    Adafruit_NeoPixel* m_strip;
};
