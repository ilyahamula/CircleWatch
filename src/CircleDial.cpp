#include "CircleDial.h"
#include "Digit.h"

#include <Adafruit_NeoPixel.h>

#define LED_COUNT 111
#define DEFAULT_BRIGHTNESS 100

CircleDial::CircleDial(uint8_t pin)
    : m_strip(new Adafruit_NeoPixel(LED_COUNT, pin, NEO_GRB + NEO_KHZ800))
    , m_currHourDigit(nullptr)
    , m_currMinDigit(nullptr)
{
    m_strip->begin();
    m_strip->setBrightness(DEFAULT_BRIGHTNESS);
    m_strip->show(); // Initialize all pixels to 'off'

    m_digits[0] = new Digit(43, 50, *m_strip); // 12
    m_hoursDigits[0] = m_digits[0];

    m_digits[1] = new Digit(42, *m_strip);
    m_digits[2] = new Digit(41, *m_strip);
    m_digits[3] = new Digit(40, *m_strip);
    m_digits[4] = new Digit(39, *m_strip);
    m_digits[5] = new Digit(37, 38, *m_strip); // 1
    m_hoursDigits[1] = m_digits[5];

    m_digits[6] = new Digit(36, *m_strip);
    m_digits[7] = new Digit(35, *m_strip);
    m_digits[8] = new Digit(34, *m_strip);
    m_digits[9] = new Digit(33, *m_strip);
    m_digits[10] = new Digit(29, 32, *m_strip); // 2
    m_hoursDigits[2] = m_digits[10];

    m_digits[11] = new Digit(28, *m_strip);
    m_digits[12] = new Digit(27, *m_strip);
    m_digits[13] = new Digit(26, *m_strip);
    m_digits[14] = new Digit(25, *m_strip);
    m_digits[15] = new Digit(20, 24, *m_strip); // 3
    m_hoursDigits[3] = m_digits[15];

    m_digits[16] = new Digit(19, *m_strip);
    m_digits[17] = new Digit(18, *m_strip);
    m_digits[18] = new Digit(17, *m_strip);
    m_digits[19] = new Digit(16, *m_strip);
    m_digits[20] = new Digit(11, 15, *m_strip); // 4
    m_hoursDigits[4] = m_digits[20];

    m_digits[21] = new Digit(10, *m_strip);
    m_digits[22] = new Digit(9, *m_strip);
    m_digits[23] = new Digit(8, *m_strip);
    m_digits[24] = new Digit(7, *m_strip);
    m_digits[25] = new Digit(4, 6, *m_strip); // 5
    m_hoursDigits[5] = m_digits[25];

    m_digits[26] = new Digit(3, *m_strip);
    m_digits[27] = new Digit(2, *m_strip);
    m_digits[28] = new Digit(1, *m_strip);
    m_digits[29] = new Digit(0, *m_strip);
    m_digits[30] = new Digit(106, 110, *m_strip); // 6
    m_hoursDigits[6] = m_digits[30];

    m_digits[31] = new Digit(105, *m_strip);
    m_digits[32] = new Digit(104, *m_strip);
    m_digits[33] = new Digit(103, *m_strip);
    m_digits[34] = new Digit(102, *m_strip);
    m_digits[35] = new Digit(95, 101, *m_strip); // 7
    m_hoursDigits[7] = m_digits[35];

    m_digits[36] = new Digit(94, *m_strip);
    m_digits[37] = new Digit(93, *m_strip);
    m_digits[38] = new Digit(92, *m_strip);
    m_digits[39] = new Digit(91, *m_strip);
    m_digits[40] = new Digit(83, 90, *m_strip); // 8
    m_hoursDigits[8] = m_digits[40];

    m_digits[41] = new Digit(82, *m_strip);
    m_digits[42] = new Digit(81, *m_strip);
    m_digits[43] = new Digit(80, *m_strip);
    m_digits[44] = new Digit(79, *m_strip);
    m_digits[45] = new Digit(73, 78, *m_strip); // 9
    m_hoursDigits[9] = m_digits[45];

    m_digits[46] = new Digit(72, *m_strip);
    m_digits[47] = new Digit(71, *m_strip);
    m_digits[48] = new Digit(70, *m_strip);
    m_digits[49] = new Digit(69, *m_strip);
    m_digits[50] = new Digit(65, 68, *m_strip); // 10
    m_hoursDigits[10] = m_digits[50];

    m_digits[51] = new Digit(64, *m_strip);
    m_digits[52] = new Digit(63, *m_strip);
    m_digits[53] = new Digit(62, *m_strip);
    m_digits[54] = new Digit(61, *m_strip);
    m_digits[55] = new Digit(55, 60, *m_strip); // 11
    m_hoursDigits[11] = m_digits[55];

    m_digits[56] = new Digit(54, *m_strip);
    m_digits[57] = new Digit(53, *m_strip);
    m_digits[58] = new Digit(52, *m_strip);
    m_digits[59] = new Digit(51, *m_strip);
}

CircleDial::~CircleDial()
{
    for (int i = 0; i < MINUTES; ++i)
    {
        delete m_digits[i];
        m_digits[i] = nullptr;
    }

    for (int i = 0; i < HOURS; ++i)
        m_hoursDigits[i] = nullptr;

    m_currHourDigit = nullptr;
    m_currMinDigit = nullptr;
}

bool CircleDial::SetTime(uint8_t hour, uint8_t min)
{
    if (hour > 23 || min > 59)
        return false;

    hour = (hour >= 12 ? hour - 12 : hour);

    if (m_currHourDigit != nullptr)
        m_currHourDigit->TurnOff();
    if (m_currMinDigit != nullptr)
        m_currMinDigit->TurnOff();

    m_currHourDigit = m_hoursDigits[hour];
    m_currMinDigit = m_digits[min];
    if (m_currHourDigit == m_currMinDigit)
        m_currMinDigit = m_digits[min - 1];

    m_currHourDigit->SetColor(m_hourColor.red, m_hourColor.green, m_hourColor.blue);
    m_currHourDigit->TurnOn();

    if (min != 0)
    {
        m_currMinDigit->SetColor(m_minuteColor.red, m_minuteColor.green, m_minuteColor.blue);
        m_currMinDigit->TurnOn();
    }
    else
        m_currMinDigit->TurnOff();

    m_strip->show();
    return true;
}

void CircleDial::SetBrightness(uint8_t value)
{
    m_strip->setBrightness(value);
    m_strip->show();
}

void CircleDial::SetHoursColor(sRGB value)
{
    m_hourColor = value;
}

void CircleDial::SetMinutesColor(sRGB value)
{
    m_minuteColor = value;
}
