#pragma once

#include "Defines.h"

class Digit;
class sRGB;

class Segment
{
    Digit* m_digits[NUM_SEG_DIGITS];
public:
    Segment(Digit* first, Digit* second, Digit* third,
        Digit* fourth, Digit* fifth);
    void Off();
    void SetCurrentMinutes(uint8_t minutes, const sRGB& color);
    Digit* GetHourDigit();
};