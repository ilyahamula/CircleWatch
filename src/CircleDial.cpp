#include "CircleDial.h"
#include "Digit.h"
#include "Segment.h"
#include "FlashMemory.h"

#include <Adafruit_NeoPixel.h>
#define MAX_SUB_PIXELS_CNT 4

CircleDial::CircleDial(uint8_t pin)
    : m_strip(new Adafruit_NeoPixel(DIAL_LED_COUNT, pin, NEO_GRB + NEO_KHZ800))
    , m_currHourDigit(nullptr)
    , m_currMinDigit(nullptr)
    , m_currSeg(nullptr)
    , m_currHour(NOT_DEFINED_TIME)
    , m_currMin(NOT_DEFINED_TIME)
    , m_mode(static_cast<eDialMode>(FLASHMEM.Read(eAddres::ModeAddr)))
    , m_forceUpdate(false)
    , m_hourColor(FLASHMEM.ReadColor(eAddres::HoursColorAddr))
    , m_minuteColor(FLASHMEM.ReadColor(eAddres::MinutesColorAddr))
    , m_brightness(FLASHMEM.Read(eAddres::DialBrightness))
{
    pinMode(MOSFET_DIAL_PIN, OUTPUT);
    digitalWrite(MOSFET_DIAL_PIN, HIGH);
    
    m_strip->begin();
    SetBrightness(m_brightness == 0 ? DEFAULT_BRIGHTNESS : m_brightness);
    m_strip->show(); // Initialize all pixels to 'off'

    uint8_t subPixels[MAX_SUB_PIXELS_CNT];

//-----12---------------------------------------------------------
    subPixels[0] = 43;
    subPixels[1] = 46;
    subPixels[2] = 47;
    subPixels[3] = 50;
    m_digits[0] = new Digit(43, 50, subPixels, 4, *m_strip); // 12
    m_hoursDigits[0] = m_digits[0];

    m_digits[1] = new Digit(42, *m_strip);
    m_digits[2] = new Digit(41, *m_strip);
    m_digits[3] = new Digit(40, *m_strip);
    m_digits[4] = new Digit(39, *m_strip);

    m_segments[0] = new Segment {m_digits[0], m_digits[1],
        m_digits[2], m_digits[3], m_digits[4]};

//-----1----------------------------------------------------------
    subPixels[0] = 38;
    m_digits[5] = new Digit(37, 38, subPixels, 1, *m_strip); // 1
    m_hoursDigits[1] = m_digits[5];

    m_digits[6] = new Digit(36, *m_strip);
    m_digits[7] = new Digit(35, *m_strip);
    m_digits[8] = new Digit(34, *m_strip);
    m_digits[9] = new Digit(33, *m_strip);

    m_segments[1] = new Segment {m_digits[5], m_digits[6],
        m_digits[7], m_digits[8], m_digits[9]};

//-----2----------------------------------------------------------
    subPixels[0] = 29;
    subPixels[1] = 32;
    m_digits[10] = new Digit(29, 32, subPixels, 2, *m_strip); // 2
    m_hoursDigits[2] = m_digits[10];

    m_digits[11] = new Digit(28, *m_strip);
    m_digits[12] = new Digit(27, *m_strip);
    m_digits[13] = new Digit(26, *m_strip);
    m_digits[14] = new Digit(25, *m_strip);

    m_segments[2] = new Segment {m_digits[10], m_digits[11],
        m_digits[12], m_digits[13], m_digits[14]};

//-----3----------------------------------------------------------
    subPixels[0] = 21;
    subPixels[1] = 24;
    m_digits[15] = new Digit(20, 24, subPixels, 2, *m_strip); // 3
    m_hoursDigits[3] = m_digits[15];

    m_digits[16] = new Digit(19, *m_strip);
    m_digits[17] = new Digit(18, *m_strip);
    m_digits[18] = new Digit(17, *m_strip);
    m_digits[19] = new Digit(16, *m_strip);

    m_segments[3] = new Segment {m_digits[15], m_digits[16],
        m_digits[17], m_digits[18], m_digits[19]};

//-----4----------------------------------------------------------
    subPixels[0] = 12;
    subPixels[1] = 15;
    m_digits[20] = new Digit(11, 15, subPixels, 2, *m_strip); // 4
    m_hoursDigits[4] = m_digits[20];

    m_digits[21] = new Digit(10, *m_strip);
    m_digits[22] = new Digit(9, *m_strip);
    m_digits[23] = new Digit(8, *m_strip);
    m_digits[24] = new Digit(7, *m_strip);

    m_segments[4] = new Segment {m_digits[20], m_digits[21],
        m_digits[22], m_digits[23], m_digits[24]};

//-----5----------------------------------------------------------
    subPixels[0] = 4;
    m_digits[25] = new Digit(4, 6, subPixels, 1, *m_strip); // 5
    m_hoursDigits[5] = m_digits[25];

    m_digits[26] = new Digit(3, *m_strip);
    m_digits[27] = new Digit(2, *m_strip);
    m_digits[28] = new Digit(1, *m_strip);
    m_digits[29] = new Digit(0, *m_strip);

    m_segments[5] = new Segment {m_digits[25], m_digits[26],
        m_digits[27], m_digits[28], m_digits[29]};

//-----6----------------------------------------------------------
    subPixels[0] = 106;
    subPixels[1] = 110;
    m_digits[30] = new Digit(106, 110, subPixels, 2, *m_strip); // 6
    m_hoursDigits[6] = m_digits[30];

    m_digits[31] = new Digit(105, *m_strip);
    m_digits[32] = new Digit(104, *m_strip);
    m_digits[33] = new Digit(103, *m_strip);
    m_digits[34] = new Digit(102, *m_strip);

    m_segments[6] = new Segment {m_digits[30], m_digits[31],
        m_digits[32], m_digits[33], m_digits[34]};

//-----7----------------------------------------------------------
    subPixels[0] = 95;
    subPixels[1] = 97;
    subPixels[2] = 101;
    m_digits[35] = new Digit(95, 101, subPixels, 3, *m_strip); // 7
    m_hoursDigits[7] = m_digits[35];

    m_digits[36] = new Digit(94, *m_strip);
    m_digits[37] = new Digit(93, *m_strip);
    m_digits[38] = new Digit(92, *m_strip);
    m_digits[39] = new Digit(91, *m_strip);

    m_segments[7] = new Segment {m_digits[35], m_digits[36],
        m_digits[37], m_digits[38], m_digits[39]};

//-----8----------------------------------------------------------
    subPixels[0] = 83;
    subPixels[1] = 87;
    subPixels[2] = 90;
    m_digits[40] = new Digit(83, 90, subPixels, 3, *m_strip); // 8
    m_hoursDigits[8] = m_digits[40];

    m_digits[41] = new Digit(82, *m_strip);
    m_digits[42] = new Digit(81, *m_strip);
    m_digits[43] = new Digit(80, *m_strip);
    m_digits[44] = new Digit(79, *m_strip);

    m_segments[8] = new Segment {m_digits[40], m_digits[41],
        m_digits[42], m_digits[43], m_digits[44]};

//-----9----------------------------------------------------------
    subPixels[0] = 73;
    subPixels[1] = 76;
    subPixels[2] = 78;
    m_digits[45] = new Digit(73, 78, subPixels, 3, *m_strip); // 9
    m_hoursDigits[9] = m_digits[45];

    m_digits[46] = new Digit(72, *m_strip);
    m_digits[47] = new Digit(71, *m_strip);
    m_digits[48] = new Digit(70, *m_strip);
    m_digits[49] = new Digit(69, *m_strip);

    m_segments[9] = new Segment {m_digits[45], m_digits[46],
        m_digits[47], m_digits[48], m_digits[49]};

//-----10---------------------------------------------------------
    subPixels[0] = 65;
    subPixels[1] = 68;
    m_digits[50] = new Digit(65, 68, subPixels, 2, *m_strip); // 10
    m_hoursDigits[10] = m_digits[50];

    m_digits[51] = new Digit(64, *m_strip);
    m_digits[52] = new Digit(63, *m_strip);
    m_digits[53] = new Digit(62, *m_strip);
    m_digits[54] = new Digit(61, *m_strip);

    m_segments[10] = new Segment {m_digits[50], m_digits[51],
        m_digits[52], m_digits[53], m_digits[54]};

//-----11---------------------------------------------------------
    subPixels[0] = 56;
    subPixels[1] = 57;
    subPixels[2] = 60;
    m_digits[55] = new Digit(55, 60, subPixels, 3, *m_strip); // 11
    m_hoursDigits[11] = m_digits[55];

    m_digits[56] = new Digit(54, *m_strip);
    m_digits[57] = new Digit(53, *m_strip);
    m_digits[58] = new Digit(52, *m_strip);
    m_digits[59] = new Digit(51, *m_strip);

    m_segments[11] = new Segment {m_digits[55], m_digits[56],
        m_digits[57], m_digits[58], m_digits[59]};
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

    if (m_currHour == hour && m_currMin == min && !m_forceUpdate)
        return true;

    OffCurrent();

    if (m_mode == eDialMode::DigitHour)
        SetDigitHourTime(hour, min);
    else if (m_mode == eDialMode::SegmentHour)
        SetSegmentHourTime(hour, min);

    m_forceUpdate = false;
    m_currHour = hour;
    m_currMin = min;

    return true;
}

void CircleDial::SetBrightness(uint8_t value)
{
    m_brightness = value;
    m_strip->setBrightness(m_brightness);
    m_forceUpdate = true;
    FLASHMEM.Write(eAddres::DialBrightness, m_brightness);
}

void CircleDial::SetHoursColor(const sRGB& value)
{
    m_hourColor = value;
    m_forceUpdate = true;
    FLASHMEM.WriteColor(eAddres::HoursColorAddr, m_hourColor);
}

void CircleDial::SetMinutesColor(const sRGB& value)
{
    m_minuteColor = value;
    m_forceUpdate = true;
    FLASHMEM.WriteColor(eAddres::MinutesColorAddr, m_minuteColor);
}

void CircleDial::SetMode(eDialMode mode)
{
    m_mode = mode;
    m_forceUpdate = true;
    FLASHMEM.Write(eAddres::ModeAddr, static_cast<uint8_t>(m_mode));
}

void CircleDial::Show()
{
    m_strip->show();
}

void CircleDial::SetDigitHourTime(uint8_t hour, uint8_t min)
{
    m_currHourDigit = m_hoursDigits[hour];
    m_currMinDigit = m_digits[min];

    m_currHourDigit->SetColor(m_hourColor);
    if (m_currHourDigit == m_currMinDigit)
        m_currHourDigit->SetSubColor(m_minuteColor);
    else
        m_currMinDigit->SetColor(m_minuteColor);
}

void CircleDial::SetSegmentHourTime(uint8_t hour, uint8_t min)
{
    m_currSeg = m_segments[hour];
    m_currSeg->SetCurrentMinutes(min, m_hourColor);

    auto curHourDigit = m_currSeg->GetHourDigit();
    m_currMinDigit = m_digits[min];
    if (curHourDigit == m_currMinDigit)
        curHourDigit->SetSubColor(m_minuteColor);
    else
        m_currMinDigit->SetColor(m_minuteColor);
}

void CircleDial::OffCurrent()
{
    if (m_currSeg != nullptr)
    {
        m_currSeg->Off();
        m_currSeg = nullptr;
    }
    if (m_currHourDigit != nullptr)
    {
        m_currHourDigit->TurnOff();
        m_currHourDigit = nullptr;
    }
    if (m_currMinDigit != nullptr)
    {
        m_currMinDigit->TurnOff();
        m_currHourDigit = nullptr;
    }
}

void CircleDial::TurnOff()
{
    m_strip->clear();
    m_strip->show();
}
