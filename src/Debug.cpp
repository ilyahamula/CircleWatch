#include "Debug.h"

#ifdef DEBUG
bool Debug::m_serialSetup = false;
int Debug::minute = 0;
#endif

void Debug::Print(String& str)
{
#ifdef DEBUG
    if (!m_serialSetup)
    {
        Serial.begin(9600);
		delay(1000);
        m_serialSetup = true;
    }
	Serial.print(str);
#endif
}

void Debug::PrintTime(const int8_t hours, const int8_t min)
{
#ifdef DEBUG
    if (!m_serialSetup)
    {
        Serial.begin(9600);
        m_serialSetup = true;
    }

	if (min != minute)
	{
		minute = min;
		Serial.print("\n");
		Serial.print(hours);
		Serial.print(" : ");
		Serial.println(min);
	}
#endif
}

