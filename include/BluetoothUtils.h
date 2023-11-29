#pragma once

#include "Defines.h"

class BLECommandReader;

struct BluetoothLE
{
    static void Init();
    static void Deinit();
private:
    static TaskHandle_t task;
};
