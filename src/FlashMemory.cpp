#include "FlashMemory.h"
#include "sRGB.h"

#include <EEPROM.h>

FlashMemory::FlashMemory()
{
    EEPROM.begin(static_cast<size_t>(eAddres::Size));
}

FlashMemory& FlashMemory::Instance()
{
    static FlashMemory inst;
    return inst;
}

void FlashMemory::Write(eAddres addres, unsigned char value)
{
    EEPROM.write(static_cast<int>(addres), value);
    EEPROM.commit();
}

void FlashMemory::WriteColor(eAddres addres, const sRGB& color)
{
    EEPROM.write(static_cast<int>(addres), color.red);
    EEPROM.write(static_cast<int>(addres) + 1, color.green);
    EEPROM.write(static_cast<int>(addres) + 2, color.blue);
    EEPROM.commit();
}

unsigned char FlashMemory::Read(eAddres addres)
{
    return EEPROM.read(static_cast<int>(addres));
}

sRGB FlashMemory::ReadColor(eAddres addres)
{
    sRGB ret;
    ret.red = EEPROM.read(static_cast<int>(addres));
    ret.green = EEPROM.read(static_cast<int>(addres) + 1);
    ret.blue = EEPROM.read(static_cast<int>(addres) + 2);
    return ret;
}
