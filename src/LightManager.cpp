#include "LightManager.h"
#include "Command.h"

#include <Adafruit_NeoPixel.h>

namespace
{
    // Fill the dots one after the other with a color
    void colorWipe(uint32_t c, uint8_t wait, Adafruit_NeoPixel& strip)
    {
        for(uint16_t i = 0; i < strip.numPixels(); i++)
        {
            strip.setPixelColor(i, c);
            strip.show();
            delay(wait);
        }
    }

    // Input a value 0 to 255 to get a color value.
    // The colours are a transition r - g - b - back to r.
    uint32_t Wheel(byte WheelPos, Adafruit_NeoPixel& strip)
    {
        WheelPos = 255 - WheelPos;
        if(WheelPos < 85)
            return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
        if(WheelPos < 170)
        {
            WheelPos -= 85;
            return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
        }
 
        WheelPos -= 170;
        return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }

    void rainbow(uint8_t wait,  Adafruit_NeoPixel& strip)
    {
        uint16_t i, j;
        for(j = 0; j < 256; j++)
        {
            for(i = 0; i < strip.numPixels(); i++)
            {
                strip.setPixelColor(i, Wheel((i+j) & 255, strip));
            }
            strip.show();
            delay(wait);
        }
    }

    // Slightly different, this makes the rainbow equally distributed throughout
    void rainbowCycle(uint8_t wait,  Adafruit_NeoPixel& strip)
    {
        uint16_t i, j;
        for(j = 0; j < 256 * 5; j++) // 5 cycles of all colors on wheel
        {
            for(i=0; i< strip.numPixels(); i++)
            {
                strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255, strip));
            }
            strip.show();
            delay(wait);
        }
    }

    //Theatre-style crawling lights.
    void theaterChase(uint32_t c, uint8_t wait,  Adafruit_NeoPixel& strip)
    {
        for (int j = 0; j < 10; j++) //do 10 cycles of chasing
        {
            for (int q = 0; q < 3; q++)
            {
                for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
                {
                    strip.setPixelColor(i+q, c);   //turn every third pixel on
                }
                strip.show();

                delay(wait);

                for (uint16_t i=0; i < strip.numPixels(); i=i+3)
                {
                    strip.setPixelColor(i+q, 0);   //turn every third pixel off
                }
            }
        }
    }

    //Theatre-style crawling lights with rainbow effect
    void theaterChaseRainbow(uint8_t wait,  Adafruit_NeoPixel& strip)
    {
        for (uint8_t j = 0; j < 256; j++) // cycle all 256 colors in the wheel
        {
            for (uint8_t q = 0; q < 3; q++)
            {
                for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
                {
                    strip.setPixelColor(i+q, Wheel( (i+j) % 255, strip));    //turn every third pixel on
                }
                strip.show();

                delay(wait);

                for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
                {
                    strip.setPixelColor(i + q, 0);  //turn every third pixel off
                }
            }
        }
    }

    void runRainbowWheel(Adafruit_NeoPixel& strip)
    {
        // Some example procedures showing how to display to the pixels:
        colorWipe(strip.Color(255, 0, 0), 50, strip); // Red
        colorWipe(strip.Color(0, 255, 0), 50, strip); // Green
        colorWipe(strip.Color(0, 0, 255), 50, strip); // Blue
        colorWipe(strip.Color(0, 0, 0, 255), 50, strip); // White RGBW
        // Send a theater pixel chase in...
    
        theaterChase(strip.Color(127, 127, 127), 50, strip); // White
        theaterChase(strip.Color(127, 0, 0), 50, strip); // Red
        theaterChase(strip.Color(0, 0, 127), 50, strip); // Blue

        rainbow(20, strip);
        rainbowCycle(20, strip);
        theaterChaseRainbow(50, strip);
    }

    Adafruit_NeoPixel* createStrip(uint8_t pin, uint8_t ledNum)
    {
        Adafruit_NeoPixel* strip = new Adafruit_NeoPixel(ledNum, pin, NEO_GRB + NEO_KHZ800);
        strip->begin();
        strip->setBrightness(DEFAULT_BRIGHTNESS);
        strip->show();
        return strip;
    }
}

void RunRainbowTask(void* params)
{
    auto strip = reinterpret_cast<Adafruit_NeoPixel*>(params);
    if (strip)
    {
        runRainbowWheel(*strip);
    }
}

void LightManager::Test(uint8_t pin, uint8_t ledNum)
{
    static Adafruit_NeoPixel* strip = createStrip(pin, ledNum);
    runRainbowWheel(*strip);
}

LightManager::LightManager(uint8_t pin)
    : m_strip(createStrip(pin, LIGHT_LED_COUNT))
    , m_outerColor(255, 255, 255)
    , m_innerColor(255, 255, 255)
    , m_mode(eLightMode::Off)
    , m_lightTask(nullptr)
    , m_update(true)
{
    m_strip->begin();
    m_strip->setBrightness(DEFAULT_BRIGHTNESS);
    m_strip->show(); // Initialize all pixels to 'off'
}

LightManager::~LightManager()
{
    delete m_strip;
}

void LightManager::SetBrightness(uint8_t value)
{
    if (m_mode == eLightMode::RainbowWheel || m_mode == eLightMode::Off)
        return;
    m_brightness = value;
    m_strip->setBrightness(value);
}

void LightManager::SetOuterColor(const sRGB& color)
{
    m_outerColor = color;
}

void LightManager::SetInnerColor(const sRGB& color)
{
    m_innerColor = color;
}

void LightManager::SetMode(const eLightMode mode)
{
    m_mode = mode;
    SetUpdate(true);
}

void LightManager::SetUpdate(const bool update)
{
    m_update = update;
}

void LightManager::Run()
{
    if (!m_update)
        return;
    m_update = false;

    switch (m_mode)
    {
    case eLightMode::WhiteCold:
    {
        KillTaskIfExist();
        SetInnerColor({255, 255, 255});
        SetOuterColor({255, 255, 255});
        RunNormalMode();
        break;
    }
    case eLightMode::WhiteWarm:
    {
        KillTaskIfExist();
        SetInnerColor({255, 160, 140});
        SetOuterColor({255, 160, 140});
        RunNormalMode();
        break;
    }
    case eLightMode::Custom:
    {
        KillTaskIfExist();
        RunNormalMode();
        break;
    }
    case eLightMode::SmoothTransfusion:
    {
        KillTaskIfExist();
        RunSmoothTransfusionMode();
        break;
    }
    case eLightMode::RainbowWheel:
    {
        RunRainbowWheelMode();
        break;
    }
    case eLightMode::Off:
    default:
    {
        KillTaskIfExist();
        Off();
        break;
    }
    }
}

void LightManager::RunNormalMode()
{
    for(uint8_t i = 0; i <= LIGHT_OUTER_END_IDX; i++)
    {
        m_strip->setPixelColor(i, m_outerColor.red, m_outerColor.green, m_outerColor.blue);
    }
    for(uint8_t i = LIGHT_OUTER_END_IDX + 1; i < m_strip->numPixels(); i++)
    {
        m_strip->setPixelColor(i, m_innerColor.red, m_innerColor.green, m_innerColor.blue);
    }
    m_strip->show();
}

void LightManager::RunSmoothTransfusionMode()
{

}

void LightManager::RunRainbowWheelMode()
{
    xTaskCreatePinnedToCore(
        RunRainbowTask,   /* Task function. */
        "light manager",     /* name of task. */
        10000,       /* Stack size of task */
        m_strip,        /* parameter of the task */
        1,           /* priority of the task */
        &m_lightTask,      /* Task handle to keep track of created task */
        1);          /* pin task to core 0 */ 
}

void LightManager::Off()
{
    m_strip->clear();
    m_strip->show();
}

void LightManager::KillTaskIfExist()
{
    if (m_lightTask)
    {
        vTaskSuspend(m_lightTask);
        vTaskDelete(m_lightTask);
        m_lightTask = nullptr;
    }
}
