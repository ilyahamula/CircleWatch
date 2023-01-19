#pragma once

#include "Defines.h"
#include "sRGB.h"

class Adafruit_NeoPixel;

enum class eLightMode
{
    Normal,
    SmoothTransfusion,
    RainbowWheel,
    Off,
};

class LightManager
{
private:
    LightManager(uint8_t pin);
    LightManager(const LightManager&) = delete;
    LightManager& operator=(const LightManager&) = delete;

public:
    static void Test(uint8_t pin = DIAL_PIN, uint8_t ledNum = DIAL_LED_COUNT);
    static void Init();
    static LightManager& Instance();

public:
    ~LightManager();
    void SetBrightness(uint8_t value);
    void SetOuterColor(const sRGB& color);
    void SetInnerColor(const sRGB& color);
    void SetMode(const eLightMode mode);
    void Run();

private:
    void RunNormalMode();
    void RunSmoothTransfusionMode();
    void RunRainbowWheelMode();
    void Off();

private:
    Adafruit_NeoPixel* m_strip;
    sRGB m_outerColor;
    sRGB m_innerColor;
    eLightMode m_mode;
    uint8_t m_brightness;
    TaskHandle_t m_lightTask;
};
