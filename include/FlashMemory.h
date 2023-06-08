#pragma once

class sRGB;

enum class eAddres
{
    HoursColorAddr = 0, // +2 for g,b
    MinutesColorAddr = 3, // +2 for g,b
    ModeAddr = 6,

    Size = 7,
};

class FlashMemory
{
    FlashMemory();
    FlashMemory(const FlashMemory&) = delete;
    FlashMemory& operator=(const FlashMemory&) = delete;
public:
    static FlashMemory& Instance();

    void Write(eAddres addres, unsigned char value);
    void WriteColor(eAddres addres, const sRGB& color);
    unsigned char Read(eAddres addres);
    sRGB ReadColor(eAddres addres);
};

#define FLASHMEM FlashMemory::Instance()
