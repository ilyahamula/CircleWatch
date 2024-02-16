#include "Command.h"
#include "LightManager.h"
#include "StringUtils.h"

#include <Arduino.h>

namespace
{
    struct SemaphoreLock
    {
        SemaphoreHandle_t& semaphore_;
        SemaphoreLock(SemaphoreHandle_t& semaphore)
            : semaphore_(semaphore)
        {
            xSemaphoreTake(semaphore_, portMAX_DELAY);
        }

        ~SemaphoreLock()
        {
            xSemaphoreGive(semaphore_);
        }
    };
}

#ifdef USE_BUTTONS
void RunCommandTracker(void* parametrs)
{
    pinMode(HOUR_BTN_PIN, INPUT_PULLDOWN);
    pinMode(MIN_BTN_PIN, INPUT_PULLDOWN);

    bool needRelese = false;
    uint8_t twoBtnClickCounter = 0;

    while (true)
    {
        if (!digitalRead(HOUR_BTN_PIN) && !digitalRead(MIN_BTN_PIN) && !needRelese)
        {
            delay(LONG_PRESS_TIME);
            if (!digitalRead(HOUR_BTN_PIN) && !digitalRead(MIN_BTN_PIN))
            {
                if (++twoBtnClickCounter == 1)
                    LightManager::Instance().SetMode(eLightMode::Normal);
                else if (twoBtnClickCounter == 2)
                    LightManager::Instance().SetMode(eLightMode::SmoothTransfusion);
                else if (twoBtnClickCounter == 3)
                    LightManager::Instance().SetMode(eLightMode::RainbowWheel);
                else if (twoBtnClickCounter == 4)
                {
                    LightManager::Instance().SetMode(eLightMode::Off);
                    twoBtnClickCounter = 0;
                }
            }
            needRelese = true;
        }
        else if (!digitalRead(HOUR_BTN_PIN) && digitalRead(MIN_BTN_PIN) && !needRelese)
        {
            delay(SHORT_PRESS_TIME);
            if (digitalRead(HOUR_BTN_PIN) && digitalRead(MIN_BTN_PIN))
                Command::InitOrInst().SetCommand(eCommand::AddHour);
            else
                needRelese = true;
        }
        else if (!digitalRead(MIN_BTN_PIN) && digitalRead(HOUR_BTN_PIN) && !needRelese)
        {
            delay(SHORT_PRESS_TIME);
            if (digitalRead(MIN_BTN_PIN) && digitalRead(HOUR_BTN_PIN))
                Command::InitOrInst().SetCommand(eCommand::AddMin);
            else
                needRelese = true;
        }
        else if (digitalRead(MIN_BTN_PIN) && digitalRead(HOUR_BTN_PIN))
            needRelese = false;
    }
}
#endif

Command::Command()
    : m_currCmd(eCommand::None)
    , m_semaphore(xSemaphoreCreateMutex())
{
}

Command& Command::InitOrInst()
{
    static Command inst;
    return inst;
}

#ifdef DEBUG
String Command::CmdToString(eCommand cmd)
{
    switch(cmd)
    {
    case eCommand::AddHour:
        return "AddHour";
    case eCommand::SubHour:
        return "SubHour";
    case eCommand::SetTime:
        return "SetTime";
    case eCommand::SetHourColor:
        return "SetHourColor";
    case eCommand::SetMinColor:
        return "SetMinColor";
    case eCommand::LightOff:
        return "LightOff";
    case eCommand::LightBrightness:
        return "LightBrightness";
    case eCommand::LightCustom:
        return "LightCustom";
    case eCommand::LightRainbow:
        return "LightRainbow";
    case eCommand::LightWhiteCold:
        return "LightWhiteCold";
    case eCommand::LightWhiteWarm:
        return "LightWhiteWarm";
    case eCommand::LightSmoothe:
        return "LightSmoothe";
    case eCommand::DialDigitHour:
        return "DialDigitHour";
    case eCommand::DialSegmentHour:
        return "DialSegmentHour";
    case eCommand::DialDefaultColors:
        return "DialDefaultColors";
    case eCommand::None:
        return "None";
    default:
        return "";
    }
}
#endif

const eCommand Command::GetCommand()
{
    SemaphoreLock lock(m_semaphore);
    eCommand tempCmd = m_currCmd;
    m_currCmd = eCommand::None;
    return tempCmd;
}

void Command::SetCommand(eCommand cmd)
{
    m_currCmd = cmd;
}

void Command::ParseCommand(const String& text)
{
    SemaphoreLock lock(m_semaphore);

    #ifdef DEBUG
    Serial.print("Got command: ");
    Serial.println(text);
    #endif

    String cmd, params;
    StringUtils::ParseCmdAndParams(text, cmd, params);

    // lambdas ------------------------------------------------------------
    auto parseTimeCmd = [this, &params](const eCommand cmd)
    {
        if (!StringUtils::ParseTime(params, m_hours, m_min, m_sec))
        {
            #ifdef DEBUG
            Serial.println("Failed to parse time!!!");
            #endif
        }
        m_currCmd = cmd;
    };

    auto parseColorCmd = [this, &params](const eCommand cmd)
    {
        if (!StringUtils::ParseColor(params, m_color))
        {
            #ifdef DEBUG
            Serial.println("Failed to parse color!!!");
            #endif
        }
        m_currCmd = cmd;
    };

    auto parseIntValueCmd = [this, &params](const eCommand cmd)
    {
        if (!StringUtils::ParseIntNumber(params, m_intValue))
        {
            #ifdef DEBUG
            Serial.println("Failed to parse int value!!!");
            #endif
        }
        else
            m_currCmd = cmd;
    };
    // end lambdas --------------------------------------------------------
    
    if (cmd == SET_TIME)
        parseTimeCmd(eCommand::SetTime);

    else if (cmd == RESTART)
        ESP.restart();
    else if (cmd == ADD_HOUR)
        m_currCmd = eCommand::AddHour;
    else if (cmd == SUB_HOUR)
        m_currCmd = eCommand::SubHour;
    else if (cmd == LIGHT_OFF)
        m_currCmd = eCommand::LightOff;
    else if (cmd == LIGHT_RAINBOW)
        m_currCmd = eCommand::LightRainbow;
    else if (cmd == LIGHT_WHITE_COLD)
        m_currCmd = eCommand::LightWhiteCold;
    else if (cmd == LIGHT_WHITE_WARM)
        m_currCmd = eCommand::LightWhiteWarm;
    else if (cmd == SET_HOUR_COLOR)
        parseColorCmd(eCommand::SetHourColor);
    else if (cmd == SET_MIN_COLOR)
        parseColorCmd(eCommand::SetMinColor);
    else if (cmd == LIGHT_CUSTOM)
        parseColorCmd(eCommand::LightCustom);
    else if (cmd == LIGHT_BRIGHTNESS)
        parseIntValueCmd(eCommand::LightBrightness);
    else if (cmd == LIGHT_SMOOTHE)
        m_currCmd = eCommand::LightSmoothe;
    else if (cmd == DIAL_DIGIT_HOUR)
        m_currCmd = eCommand::DialDigitHour;
    else if (cmd == DIAL_SEGMENT_HOUR)
        m_currCmd = eCommand::DialSegmentHour;
    else if (cmd == DIAL_DEFAULT_COLORS)
        m_currCmd = eCommand::DialDefaultColors;
    else if (cmd == SET_DEEP_SLEEP_TIME)
        parseIntValueCmd(eCommand::TimeBeforeDeepSleep);
    else if (cmd == DIAL_BRIGHTNESS)
        parseIntValueCmd(eCommand::DialBrightness);
}

int Command::GetIntValue() const
{
    return m_intValue;
}

void Command::GetTime(int8_t& hours, int8_t& min, int8_t& sec) const
{
    hours = m_hours;
    min = m_min;
    sec = m_sec;
}

const sRGB& Command::GetColor() const
{
    return m_color;
}
