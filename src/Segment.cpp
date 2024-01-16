#include "Segment.h"
#include "Digit.h"
#include <cassert>

#define HOUR_DIGIT 0
#define SEG_FIRST_SEC 1
#define SEG_SECOND_SEC 2
#define SEG_THIRD_SEC 3
#define SEG_FOURTH_SEC 4

Segment::Segment(Digit* first, Digit* second, Digit* third,
        Digit* fourth, Digit* fifth)
{
    m_digits[HOUR_DIGIT] = first;
    m_digits[SEG_FIRST_SEC] = second;
    m_digits[SEG_SECOND_SEC] = third;
    m_digits[SEG_THIRD_SEC] = fourth;
    m_digits[SEG_FOURTH_SEC] = fifth;
}

void Segment::Off()
{
    for (auto& digit : m_digits)
        digit->TurnOff();
}

void Segment::SetCurrentMinutes(uint8_t minutes, const sRGB& color)
{
    if (minutes >= 0)
        m_digits[HOUR_DIGIT]->SetColor(color);
    if (minutes >= 12)
        m_digits[SEG_FIRST_SEC]->SetColor(color);
    if (minutes >= 24)
        m_digits[SEG_SECOND_SEC]->SetColor(color);
    if (minutes >= 36)
        m_digits[SEG_THIRD_SEC]->SetColor(color);
    if (minutes >= 48)
        m_digits[SEG_FOURTH_SEC]->SetColor(color);
}

Digit* Segment::GetHourDigit()
{
    return m_digits[HOUR_DIGIT];
}
