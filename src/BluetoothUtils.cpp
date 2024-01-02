#include "BluetoothUtils.h"
#include "Defines.h"

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "Command.h"
#include "DeepSleepManager.h"

#define DEVINFO_UUID              (uint16_t)0x180a
#define DEVINFO_MANUFACTURER_UUID (uint16_t)0x2a29
#define DEVINFO_NAME_UUID         (uint16_t)0x2a24
#define DEVINFO_SERIAL_UUID       (uint16_t)0x2a25

class ServerCallbacks: public BLEServerCallbacks 
{
    void onConnect(BLEServer* pServer) 
    {
        DeepSleepManager::inst().m_isUserBLEConnected = true;
        #ifdef DEBUG
        Serial.println("BLE connected device");
        #endif
    };

    void onDisconnect(BLEServer* pServer)
    {
        DeepSleepManager::inst().m_isUserBLEConnected = false;
        #ifdef DEBUG
        Serial.println("Device BLE disconnected");
        #endif
    }
#ifdef DEBUG
public:
    
    ~ServerCallbacks()
    {
        Serial.println("Server Callback object is destroyed!!!");
    }
#endif
};

class BLECommandReader : public BLECharacteristicCallbacks 
{
    void onWrite(BLECharacteristic* pCharacteristic)
    {
        std::string value1 = pCharacteristic->getValue();
        Command::InitOrInst().ParseCommand(value1.c_str());
    }
};

void RunBLE(void* params)
{
    String devName = DEVICE_NAME;
    String chipId = String((uint32_t)(ESP.getEfuseMac() >> 24), HEX);
    devName += '_';
    devName += chipId;

    BLEDevice::init(devName.c_str());
    static BLEServer *pServer = BLEDevice::createServer();
#ifdef DEBUG
    ServerCallbacks serverCallback;
    pServer->setCallbacks(&serverCallback);
#endif
    static BLEService* pService = pServer->createService(SERVICE_UUID);
    static BLECharacteristic* pElem = pService->createCharacteristic(ELEM_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

    static BLECommandReader commandReader;
    pElem->setCallbacks(&commandReader);
    pService->start();

    // ----- Advertising
    BLEAdvertising* pAdvertising = pServer->getAdvertising();

    BLEAdvertisementData adv;
    adv.setName(devName.c_str());
    pAdvertising->setAdvertisementData(adv);

    BLEAdvertisementData adv2;
    adv2.setCompleteServices(BLEUUID(SERVICE_UUID));
    pAdvertising->setScanResponseData(adv2);

    pAdvertising->start();

    while(true)
        vTaskDelay(10);
}

TaskHandle_t BluetoothLE::task = nullptr;

void BluetoothLE::Init()
{
    if (!task)
    {
        #ifdef DEBUG
        Serial.println("BLE initialized");
        #endif
        xTaskCreatePinnedToCore(
            RunBLE,   
            "command tracker",     
            10000,       
            NULL,        
            tskIDLE_PRIORITY,   
            &task,      
            tskNO_AFFINITY);
    }
}

void BluetoothLE::Deinit()
{
    if (task)
    {
        #ifdef DEBUG
        Serial.println("BLE Deinitialized");
        #endif
        vTaskSuspend(task);
        vTaskDelete(task);
        task = nullptr;
    }
}
