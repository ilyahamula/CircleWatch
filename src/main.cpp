#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "RTClib.h"
#include "CircleDial.h"

#define LIGHT_SENSOR_PIN_D 26
#define LIGHT_SENSOR_PIN_A 25

namespace
{
    void adjustBrightness(uint16_t light, CircleDial& dial)
    {
        if (light <= 1000)
            dial.SetBrightness(255);
        else if (light >= 3700)
            dial.SetBrightness(20);
        else
            dial.SetBrightness(static_cast<uint8_t>(map(100000 / light, 27, 100, 25, 250)));
    }
}

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
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop()
{
    DateTime now = rtc.now();

    int analogValue = analogRead(LIGHT_SENSOR_PIN_A);
    adjustBrightness(analogValue, *dial);
    // We'll have a few threshholds, qualitatively determined
    

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
    Serial.println();
    Serial.println();
  
    dial->SetTime(now.hour(), now.minute());
    delay(200);
    //delay(1000);
}