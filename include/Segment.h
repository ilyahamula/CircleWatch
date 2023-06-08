#pragma once

#include "Defines.h"
#include <initializer_list>
#define NUM_SEG_DIGITS 5

class Digit;
class sRGB;

class Segment
{
    Digit* m_digits[NUM_SEG_DIGITS];
public:
    Segment(const std::initializer_list<Digit*>& list);
    void Off();
    void SetCurrentMinutes(uint8_t minutes, const sRGB& color);
    Digit* GetHourDigit();
};