#pragma once

#include "Defines.h"
#include "sRGB.h"

class Adafruit_NeoPixel;

enum class eLightMode
{
    SmoothTransfusion,
    WhiteCold,
    WhiteWarm,
    RainbowWheel,
    Custom,
    Off,
};

class LightManager
{
public:
    LightManager(uint8_t pin);
    LightManager(const LightManager&) = delete;
    LightManager& operator=(const LightManager&) = delete;
    ~LightManager();

public:
    static void Test(uint8_t pin = DIAL_PIN, uint16_t ledNum = DIAL_LED_COUNT);

public:
    void SetBrightness(uint8_t value);
    void SetOuterColor(const sRGB& color);
    void SetInnerColor(const sRGB& color);
    void SetMode(const eLightMode mode);
    void SetUpdate(const bool update);
    void Run();
    void TurnOff();

private:
    void RunNormalMode();
    void RunSmoothTransfusionMode();
    void RunRainbowWheelMode();
    void Off();
    void KillTaskIfExist();

private:
    Adafruit_NeoPixel* m_strip;
    sRGB m_outerColor;
    sRGB m_innerColor;
    eLightMode m_mode;
    uint8_t m_brightness;
    TaskHandle_t m_lightTask;
    bool m_update;
};
