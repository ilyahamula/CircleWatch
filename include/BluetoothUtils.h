#pragma once

#include "Defines.h"
#include <BLEServer.h>

class BluetoothUtils: 
    public BLEServerCallbacks,
    public BLECharacteristicCallbacks
{
public:
    static void Init(const char* watchSettings);
    BluetoothUtils(const char* watchSettings);

    void onConnect(BLEServer* pServer) override;
    void onDisconnect(BLEServer* pServer) override;
    void onWrite(BLECharacteristic* pCharacteristic) override;
    void idle();
    
private:
    BLEServer* m_server;
    BLEService* m_service;
    BLECharacteristic* m_characteristic;

    static TaskHandle_t task;
};
