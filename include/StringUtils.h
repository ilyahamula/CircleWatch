#pragma once
#include <Arduino.h>

class sRGB;

namespace  StringUtils
{
    bool isNumber(const String& str);
    bool ParseIntNumber(const String& text, int& value);
    bool ParseTime(const String& text, signed char& hours, signed char& min
        ,signed char& sec);
    bool ParseColor(const String& text, sRGB& color);
    bool ParseCmdAndParams(const String& text, String& cmd, String& params);
    String ColorToString(const sRGB& color);
}
