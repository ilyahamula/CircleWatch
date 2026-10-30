#include "RelayManager.h"
#include "Defines.h"
#include <Arduino.h>

RelayManager::RelayManager()
{
    pinMode(RELAY_DIAL_PIN, RELAY_OUTPUT_MODE);
    pinMode(RELAY_LIGHT_PIN, RELAY_OUTPUT_MODE);
}

RelayManager& RelayManager::inst()
{
    static RelayManager obj;
    return obj;
}

void RelayManager::OffAll() const
{
    digitalWrite(RELAY_DIAL_PIN, RELAY_OFF);
    digitalWrite(RELAY_LIGHT_PIN, RELAY_OFF);
}

void RelayManager::OnAll() const
{
    digitalWrite(RELAY_DIAL_PIN, RELAY_ON);
    digitalWrite(RELAY_LIGHT_PIN, RELAY_ON);
    delay(200);
}

void RelayManager::OnDial() const
{
    digitalWrite(RELAY_DIAL_PIN, RELAY_ON);
    delay(200);
}

void RelayManager::OffDial() const
{
    digitalWrite(RELAY_DIAL_PIN, RELAY_OFF);
}

void RelayManager::OnLight() const
{
    digitalWrite(RELAY_LIGHT_PIN, RELAY_ON);
    delay(200);
}

void RelayManager::OffLight() const
{
    digitalWrite(RELAY_LIGHT_PIN, RELAY_OFF);
}