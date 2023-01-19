#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "RTClib.h"

#include "CircleDial.h"
#include "LightManager.h"
#include "Command.h"

namespace
{
    uint8_t adjustBrightness(uint16_t light)
    {
        if (light <= 1000)
            return 255;
        if (light >= 3700)
            return 20;
        
        return static_cast<uint8_t>(map(100000 / light, 27, 100, 25, 250));
    }

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
}

RTC_DS1307 rtc;
CircleDial* dial = nullptr;

void setup()
{
    Serial.begin(9600);
    delay(1000); // wait for console opening

    dial = new CircleDial(DIAL_PIN);
    dial->SetHoursColor({200, 200, 255});
    dial->SetMinutesColor({0, 0, 255});

    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        while (true);
    }

    LightManager::Init();
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop()
{
    DateTime now = rtc.now();

    printTimeInSerial(now);

    dial->SetTime(now.hour(), now.minute());
    dial->SetBrightness(adjustBrightness(analogRead(LIGHT_SENSOR_PIN_A)));
    dial->Show();

    switch (Command::Instance().GetCommand())
    {
    case eCommand::AddHour:
        rtc.adjust(now + 3600);
        break;
    case eCommand::AddMin:
        rtc.adjust(now + 60);
        break;
    case eCommand::None:
    default:
        break;
    }

    delay(200);
}