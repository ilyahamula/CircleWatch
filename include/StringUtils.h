#pragma once

class String;
class sRGB;

namespace  StringUtils
{
    bool isNumber(const String& str);
    bool ParseIntNumber(const String& text, int& value);
    bool ParseTime(const String& text, signed char& hours, signed char& min);
    bool ParseColor(const String& text, sRGB& color);
    bool ParseCmdAndParams(const String& text, String& cmd, String& params);
}
