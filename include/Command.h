#pragma once

#include <Arduino.h>
#include "Defines.h"
#include "sRGB.h"

enum class eCommand
{
    None,
    AddHour,
    SubHour,
    SetTime,
    SetHourColor,
    SetMinColor,
    LightOff,
    LightRainbow,
    LightWhiteCold,
    LightWhiteWarm,
    LightCustom,
    LightBrightness,
    LightSmoothe,
    DialDigitHour,
    DialSegmentHour,
    DialDefaultColors,
    TimeBeforeDeepSleep,
    DialBrightness,
};

class Command
{
    friend class BLECommandReader;

    Command();
    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

public:
#ifdef DEBUG
    static String CmdToString(eCommand cmd);
#endif
    static Command& InitOrInst();
    void SetCommand(eCommand cmd);
    const eCommand GetCommand();
    int GetIntValue() const;
    void GetTime(int8_t& hours, int8_t& min) const;
    const sRGB& GetColor() const;

private:
    void ParseCommand(const String& text);

private:
    //TaskHandle_t m_taskHndl;
    mutable SemaphoreHandle_t m_semaphore;
    eCommand m_currCmd;

private:
    int8_t m_hours;
    int8_t m_min;
    sRGB m_color;
    int m_intValue;
};
