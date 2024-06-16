#include "FlashMemory.h"
#include "sRGB.h"
#include "StringUtils.h"

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

void FlashMemory::Write(eAddres addres, unsigned char value) const
{
    EEPROM.write(static_cast<int>(addres), value);
    EEPROM.commit();
}

void FlashMemory::WriteColor(eAddres addres, const sRGB& color) const
{
    EEPROM.write(static_cast<int>(addres), color.red);
    EEPROM.write(static_cast<int>(addres) + 1, color.green);
    EEPROM.write(static_cast<int>(addres) + 2, color.blue);
    EEPROM.commit();
}

void FlashMemory::WriteDSTime(unsigned long value) const
{
    EEPROM.write(static_cast<int>(eAddres::DeepSleepTime), (value >> 0) & 0xFF);
    EEPROM.write(static_cast<int>(eAddres::DeepSleepTime) + 1, (value >> 8) & 0xFF);
    EEPROM.write(static_cast<int>(eAddres::DeepSleepTime) + 2, (value >> 16) & 0xFF);
    EEPROM.write(static_cast<int>(eAddres::DeepSleepTime) + 3, (value >> 24) & 0xFF);
    EEPROM.commit();
}

unsigned char FlashMemory::Read(eAddres addres) const
{
    return EEPROM.read(static_cast<int>(addres));
}

sRGB FlashMemory::ReadColor(eAddres addres) const
{
    sRGB ret;
    ret.red = EEPROM.read(static_cast<int>(addres));
    ret.green = EEPROM.read(static_cast<int>(addres) + 1);
    ret.blue = EEPROM.read(static_cast<int>(addres) + 2);
    return ret;
}

unsigned long FlashMemory::ReadDSTime() const
{
    uint32_t value = 0;
    value |= ((uint32_t)EEPROM.read(static_cast<int>(eAddres::DeepSleepTime)) << 0);
    value |= ((uint32_t)EEPROM.read(static_cast<int>(eAddres::DeepSleepTime) + 1) << 8);
    value |= ((uint32_t)EEPROM.read(static_cast<int>(eAddres::DeepSleepTime) + 2) << 16);
    value |= ((uint32_t)EEPROM.read(static_cast<int>(eAddres::DeepSleepTime) + 3) << 24);
    return value;
}
