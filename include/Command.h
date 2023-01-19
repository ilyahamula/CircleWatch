#pragma once

#include "Defines.h"

enum class eCommand
{
    None,
    AddHour,
    AddMin,
};

class Command
{
    Command();
    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

public:
    static Command& Instance();
    void SetCommand(eCommand cmd);
    const eCommand GetCommand();

private:
    TaskHandle_t m_buttonsTask;
    TaskHandle_t m_bluetoothTask;
    eCommand m_currCmd;
};
