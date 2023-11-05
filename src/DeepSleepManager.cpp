#include "DeepSleepManager.h"
#include "Defines.h"
#include "CircleDial.h"
#include <Arduino.h>

namespace
{
    uint32_t additionalTime = 0;
    inline uint32_t getComputedTime()
    {
        return millis() - additionalTime;
    }

    void IRAM_ATTR ISR()
    {
        additionalTime = millis();
    } 
}

DeepSleepManager::DeepSleepManager()
    : m_isUserBLEConnected(false)
    , m_isLighthOn(false)
    , m_activeTime(WORKING_TIME)
{
    pinMode(WAKEUP_PIN, INPUT_PULLUP);
    attachInterrupt(WAKEUP_PIN, &ISR, RISING);

    auto res = esp_deep_sleep_enable_gpio_wakeup((1ULL << WAKEUP_PIN), ESP_GPIO_WAKEUP_GPIO_HIGH);
    #ifdef DEBUG
    if (res != ESP_OK)
    {
        Serial.println("Failed to setup wake up pin !!!!!!!!");
    }
    #endif
}

DeepSleepManager& DeepSleepManager::inst()
{
    static DeepSleepManager obj;
    return obj;
}

void DeepSleepManager::check(CircleDial* dial)
{
    if (m_isLighthOn || m_isUserBLEConnected)
    {
        #ifdef DEBUG
        Serial.println("BLE connected or Light is On");
        #endif
        return;
    }

    if (getComputedTime() >= m_activeTime)
    {
        if (dial)
            dial->TurnOff();
        digitalWrite(MOSFET_PIN, LOW);
        esp_deep_sleep_start();
    }
}