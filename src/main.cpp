#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "RTClib.h"

#include "CircleDial.h"
#include "LightManager.h"
#include "Command.h"
#include "BluetoothUtils.h"
#include "DeepSleepManager.h"

namespace
{
#ifdef DEBUG
    void printTimeInSerial(const DateTime& now)
    {
        Serial.print(now.year(), DEC);
        Serial.print('/');
        Serial.print(now.month(), DEC);
        Serial.print('/');
        Serial.print(now.day(), DEC);
        Serial.print(" ");
        Serial.print(now.hour(), DEC);
        Serial.print(':');
        Serial.print(now.minute(), DEC);
        Serial.print(':');
        Serial.print(now.second(), DEC);
        Serial.println();
    }
#endif
}

RTC_DS3231 rtc;
CircleDial* dial = nullptr;
LightManager* light = nullptr;

void setup()
{
    dial = new CircleDial(DIAL_PIN);
#ifdef DEBUG
    Serial.begin(9600);
    delay(1000);
    Serial.println("conecting to RTC...");
#endif
    while (!rtc.begin())
    {
        delay(200);
    }
#ifdef DEBUG
    Serial.println("RTC connected!");
#endif

    light = new LightManager(LIGHT_PIN);
    BluetoothUtils::Init();
}

void loop()
{
    DeepSleepManager::inst().check(dial);
    
    DateTime now = rtc.now();
    const auto cmd = Command::InitOrInst().GetCommand();

#ifdef DEBUG
    Serial.println(Command::CmdToString(cmd));
#endif

    switch (cmd)
    {
    case eCommand::AddHour:
        rtc.adjust(now + 3600);
        break;
    case eCommand::SubHour:
        rtc.adjust(now + -3600);
        break;
    case eCommand::SetTime:
    {
        int8_t hours = NOT_DEFINED_TIME;
		int8_t min = NOT_DEFINED_TIME;
		Command::InitOrInst().GetTime(hours, min);
        rtc.adjust(DateTime(now.year(), now.month(), now.day(), hours, min, 0));
        break;
    }
    case eCommand::SetHourColor:
        dial->SetHoursColor(Command::InitOrInst().GetColor());
        break;
    case eCommand::SetMinColor:
        dial->SetMinutesColor(Command::InitOrInst().GetColor());
        break;
    case eCommand::LightOff:
        light->SetMode(eLightMode::Off);
        break;
    case eCommand::LightRainbow:
        light->SetMode(eLightMode::RainbowWheel);
        break;
    case eCommand::LightWhiteCold:
        light->SetMode(eLightMode::WhiteCold);
        break;
    case eCommand::LightWhiteWarm:
        light->SetMode(eLightMode::WhiteWarm);
        break;
    case eCommand::LightCustom:
    {
        const auto& color = Command::InitOrInst().GetColor();
        light->SetInnerColor(color);
        light->SetOuterColor(color);
        light->SetMode(eLightMode::Custom);
        break;
    }
    case eCommand::LightBrightness:
    {
        light->SetUpdate(true);
        light->SetBrightness(Command::InitOrInst().GetIntValue());
        break;
    }
    case eCommand::LightSmoothe:
        light->SetMode(eLightMode::SmoothTransfusion);
        break;
    case eCommand::DialDigitHour:
        dial->SetMode(eDialMode::DigitHour);
        break;
    case eCommand::DialSegmentHour:
        dial->SetMode(eDialMode::SegmentHour);
        break;
    case eCommand::DialDefaultColors:
    {
        dial->SetHoursColor(HOUR_COLOR);
        dial->SetMinutesColor(MIN_COLOR);
        break;
    }
    case eCommand::TimeBeforeDeepSleep:
    {
        DeepSleepManager::inst().m_activeTime = Command::InitOrInst().GetIntValue();
        break;
    }
    case eCommand::DialBrightness:
    {
        dial->SetBrightness(static_cast<uint8_t>(Command::InitOrInst().GetIntValue()));
        break;
    }
    case eCommand::None:
    default:
        break;
    }

    dial->SetTime(now.hour(), now.minute());
    dial->Show();
    light->Run();

#ifdef DEBUG
    printTimeInSerial(now);
    delay(1000);
#endif

}