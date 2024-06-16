#pragma once
#include <WString.h>

class sRGB;

enum class eAddres
{
    HoursColorAddr = 0, // +2 for g,b
    MinutesColorAddr = 3, // +2 for g,b
    DialMode = 6,
    DialBrightness = 7,
    DeepSleepTime = 8,
    
    Size = 12,
};

class FlashMemory
{
    FlashMemory();
    FlashMemory(const FlashMemory&) = delete;
    FlashMemory& operator=(const FlashMemory&) = delete;
public:
    static FlashMemory& Instance();

    void Write(eAddres addres, unsigned char value) const;
    void WriteColor(eAddres addres, const sRGB& color) const;
    void WriteDSTime(unsigned long value) const;
    unsigned char Read(eAddres addres) const;
    sRGB ReadColor(eAddres addres) const;
    unsigned long ReadDSTime() const;
};

#define FLASHMEM FlashMemory::Instance()
