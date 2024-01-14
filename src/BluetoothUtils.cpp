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

BluetoothUtils::BluetoothUtils()
{
    String devName = DEVICE_NAME;
    String chipId = String((uint32_t)(ESP.getEfuseMac() >> 24), HEX);
    devName += '_';
    devName += chipId;

    BLEDevice::init(devName.c_str());
    m_server = BLEDevice::createServer();
    m_server->setCallbacks(this);

    m_service = m_server->createService(SERVICE_UUID);
    m_characteristic = m_service->createCharacteristic(ELEM_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

    m_characteristic->setCallbacks(this);
    m_service->start();

    // ----- Advertising
    BLEAdvertising* pAdvertising = m_server->getAdvertising();

    BLEAdvertisementData adv;
    adv.setName(devName.c_str());
    pAdvertising->setAdvertisementData(adv);

    BLEAdvertisementData adv2;
    adv2.setCompleteServices(BLEUUID(SERVICE_UUID));
    pAdvertising->setScanResponseData(adv2);

    pAdvertising->start();
}

void BluetoothUtils::onConnect(BLEServer* pServer) 
{
    DeepSleepManager::inst().m_isUserBLEConnected = true;
    #ifdef DEBUG
    Serial.println("BLE connected device");
    #endif
}

void BluetoothUtils::onDisconnect(BLEServer* pServer)
{
    DeepSleepManager::inst().m_isUserBLEConnected = false;
    #ifdef DEBUG
    Serial.println("Device BLE disconnected");
    #endif
}

void BluetoothUtils::onWrite(BLECharacteristic* pCharacteristic)
{
    std::string value1 = pCharacteristic->getValue();
    Command::InitOrInst().ParseCommand(value1.c_str());
}

void RunBLE(void* params)
{
    BluetoothUtils bleObj();

    while(true)
        vTaskDelay(10);
}

TaskHandle_t BluetoothUtils::task = nullptr;

void BluetoothUtils::Init()
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
