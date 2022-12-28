#ifndef DEBUGHEADER_H
#define DEBUGHEADER_H

#include <Arduino.h>
#define DEBUG

enum class DIAL;

class Debug
{
    Debug() = delete;

#ifdef DEBUG
    static bool m_serialSetup;
    static int minute;
#endif
public:
    template  <typename T>
    static void Print(T text)
    {
#ifdef DEBUG
        if (!m_serialSetup)
        {
            Serial.begin(9600);
            delay(300);
            m_serialSetup = true;
        }
        Serial.print(text);
#endif
    }

    static void Print(String& str);
    static void PrintTime(const int8_t hours, const int8_t min);
};

#endif