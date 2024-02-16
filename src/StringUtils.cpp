#include "StringUtils.h"
#include "Defines.h"
#include "sRGB.h"

#include <Arduino.h>

namespace StringUtils
{
    bool isNumber(const String& str)
    {
        for (const auto ch : str)
            if (!isDigit(ch))
                return false;
        return true;
    };

    bool ParseTime(const String& text, signed char& hours, signed char& min, signed char& sec)
    {
        const int pos = text.indexOf(TIME_SEPARATOR);
        if (pos == -1)
            return false;

        const String hoursStr = text.substring(0, pos);
        const String minutesSecStr = text.substring(pos + 1);
    
        const int pos1 = minutesSecStr.indexOf(TIME_SEPARATOR);
        if (pos1 == -1)
            return false;

        const String minutesStr = minutesSecStr.substring(0, pos1);
        const String secStr = minutesSecStr.substring(pos1 + 1);

        if (!isNumber(hoursStr) || !isNumber(minutesStr)
            || !isNumber(secStr))
            return false;
        
        int hoursLocal = hoursStr.toInt();
        int minLocal = minutesStr.toInt();
        int secLocal = secStr.toInt();

        if (hoursLocal > 23 || hoursLocal < 0 ||
            minLocal > 59 || minLocal < 0 ||
            secLocal > 59 || secLocal < 0)
            return false;
        hours = hoursLocal;
        min = minLocal;
        sec = secLocal;
        return true;
    }

    bool ParseIntNumber(const String& text, int& value)
    {
        if (!isNumber(text))
            return false;

        value = text.toInt();
        return true;
    }

    bool ParseCmdAndParams(const String& text, String& cmd, String& params)
    {
        const int pos = text.indexOf(CMD_PARAM_SEPARATOR);
        if (pos == -1)
        {
            cmd = text;
            return false;
        }

        cmd = text.substring(0, pos);
        params = text.substring(pos + 1);

        #ifdef DEBUG
        Serial.print("\nparse cmd: " );
        Serial.print(cmd);
        Serial.print("\nparse params: " );
        Serial.println(params);
        #endif
        return true;
    }

    bool ParseColor(const String& text, sRGB& color)
    {
        auto parseCSV = [](String& input, uint8_t& value)
        {
            #ifdef DEBUG
            Serial.print("\ninput is :" );
            Serial.print(input);
            #endif
            const int pos = input.indexOf(COLOR_VALUE_SEPARATOR);
            if (pos == -1)
            {
                #ifdef DEBUG
                Serial.println("\npos == -1" );
                #endif
                if (!isNumber(input))
                    return false;
                value = input.toInt();
                return true;
            }

            String sVal = input.substring(0, pos);
            input = input.substring(pos + 1);

            if (isNumber(sVal))
            {
                value = sVal.toInt();
                return true;
            }
            return false;
        };

        String input = text;
        if (!parseCSV(input, color.red))
            return false;

        if (!parseCSV(input, color.green))
            return false;

        if (!parseCSV(input, color.blue))
            return false;

        return true;
    }
}