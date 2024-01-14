#pragma once

#include "Defines.h"
#include <BLEServer.h>

class BluetoothUtils: 
    public BLEServerCallbacks,
    public BLECharacteristicCallbacks
{
public:
    static void Init();
    BluetoothUtils();

    void onConnect(BLEServer* pServer) override;
    void onDisconnect(BLEServer* pServer) override;
    void onWrite(BLECharacteristic* pCharacteristic) override;
    
private:
    BLEServer* m_server;
    BLEService* m_service;
    BLECharacteristic* m_characteristic;
    static TaskHandle_t task;
};
