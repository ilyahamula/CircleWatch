#include "Command.h"
#include "LightManager.h"

#include <Arduino.h>

void RunButtonManager(void* parametrs)
{
    pinMode(HOUR_BTN_PIN, INPUT_PULLDOWN);
    pinMode(MIN_BTN_PIN, INPUT_PULLDOWN);

    bool needRelese = false;
    uint8_t twoBtnClickCounter = 0;

    while (true)
    {
        if (digitalRead(HOUR_BTN_PIN) && digitalRead(MIN_BTN_PIN) && !needRelese)
        {
            delay(LONG_PRESS_TIME);
            if (digitalRead(HOUR_BTN_PIN) && digitalRead(MIN_BTN_PIN))
            {
                if (++twoBtnClickCounter == 1)
                    LightManager::Instance().SetMode(eLightMode::Normal);
                else if (twoBtnClickCounter == 2)
                    LightManager::Instance().SetMode(eLightMode::SmoothTransfusion);
                else if (twoBtnClickCounter == 3)
                    LightManager::Instance().SetMode(eLightMode::RainbowWheel);
                else if (twoBtnClickCounter == 4)
                {
                    LightManager::Instance().SetMode(eLightMode::Off);
                    twoBtnClickCounter = 0;
                }
            }
            needRelese = true;
        }
        else if (digitalRead(HOUR_BTN_PIN) && !digitalRead(MIN_BTN_PIN) && !needRelese)
        {
            delay(SHORT_PRESS_TIME);
            if (!digitalRead(HOUR_BTN_PIN) && !digitalRead(MIN_BTN_PIN))
                Command::Instance().SetCommand(eCommand::AddHour);
            else
                needRelese = true;
        }
        else if (digitalRead(MIN_BTN_PIN) && !digitalRead(HOUR_BTN_PIN) && !needRelese)
        {
            delay(SHORT_PRESS_TIME);
            if (!digitalRead(MIN_BTN_PIN) && !digitalRead(HOUR_BTN_PIN))
                Command::Instance().SetCommand(eCommand::AddMin);
            else
                needRelese = true;
        }
        else if (!digitalRead(MIN_BTN_PIN) && !digitalRead(HOUR_BTN_PIN))
            needRelese = false;
    }
}

Command::Command()
    : m_buttonsTask()
    , m_bluetoothTask()
    , m_currCmd(eCommand::None)
{
    xTaskCreatePinnedToCore(
        RunButtonManager,   /* Task function. */
        "button manager",     /* name of task. */
        10000,       /* Stack size of task */
        NULL,        /* parameter of the task */
        1,           /* priority of the task */
        &m_buttonsTask,      /* Task handle to keep track of created task */
        0);          /* pin task to core 0 */                  

    #ifdef USE_BLUETOOTH
    xTaskCreatePinnedToCore(
        RunBluetooth,   /* Task function. */
        "Bluetooth",     /* name of task. */
        10000,       /* Stack size of task */
        NULL,        /* parameter of the task */
        1,           /* priority of the task */
        &m_bluetoothTask,      /* Task handle to keep track of created task */
        1);          /* pin task to core 0 */ 
    #endif
}

Command& Command::Instance()
{
    static Command inst;
    return inst;
}

void Command::SetCommand(eCommand cmd)
{
    m_currCmd = cmd;
}

const eCommand Command::GetCommand()
{
    eCommand tempCmd = m_currCmd;
    m_currCmd = eCommand::None;
    return tempCmd;
}
