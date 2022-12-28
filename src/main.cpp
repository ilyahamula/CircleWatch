#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "RTClib.h"
#include "CircleDial.h"

RTC_DS1307 rtc;
CircleDial* dial = nullptr;

void setup()
{
    Serial.begin(9600);
    delay(3000); // wait for console opening
  
    dial = new CircleDial(12);

    sRGB hoursColor;
    hoursColor.red = 200;
    hoursColor.green = 200;
    hoursColor.blue = 255;
    dial->SetHoursColor(hoursColor);

    sRGB minColor;
    minColor.blue = 255;
    dial->SetMinutesColor(minColor);

    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        while (1);
    }

    // following line sets the RTC to the date &amp; time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop()
{
    DateTime now = rtc.now();
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
    dial->SetTime(now.hour(), now.minute());
    delay(1000);
}