#if defined(_DEBUG)
#include "pch.h"
#include <windows.h>
#include "Utility.h"
#include "BleDeviceWatcher.h"
#include "BleDeviceObject.h"
#include "UuidManager.h"
#include "UnityInterface.h"

using namespace BlePlugin;

void debugDevices(const BlePlugin::BleDeviceWatcher& watcher) {
    int deviceNum = watcher.GetDeviceNum();
    std::cout << "device " << deviceNum << std::endl;
    for (int i = 0; i < deviceNum; ++i) {
        std::cout << "  "<< std::hex << watcher.GetAddr(i) << "::" << 
            watcher.GetName(i) << "  " <<  watcher.GetRssi(i) <<
            std::endl;
    }
}

void ButtonSetSubscrive(uint64_t addr) {
    void* serviceUUID = _BlePluginGetOrCreateUuidObject(0x10B20100U, 0x5B3B4571U, 0x9508CF3EU, 0xFCD7BBAEU);

    void* charaStrics = _BlePluginGetOrCreateUuidObject(0x10B20107U,
        0x5B3B4571U, 0x9508CF3EU, 0xFCD7BBAEU);

    _BlePluginSetNotificateRequest(addr, serviceUUID, charaStrics, true);
    //    0x10B20107,0x5B3B4571 ,0x9508CF3E , 0xFCD7BBAE
}

void TestRun(){
    auto uuidMgr = BlePlugin::UuidManager::GetInstance();
    void* gattUUID = _BlePluginGetOrCreateUuidObject(0x10B20100U,
        0x5B3B4571U, 0x9508cf3eU, 0xfcd7bbaeU);

    void* serviceUUID = _BlePluginGetOrCreateUuidObject(0x10B20100U, 0x5B3B4571U, 0x9508CF3EU, 0xFCD7BBAEU);
    void* charaUUID = _BlePluginGetOrCreateUuidObject(0x10B20102U, 0x5B3B4571U, 0x9508CF3EU, 0xFCD7BBAEU);
    void* gattUUID2 = _BlePluginGetOrCreateUuidObject(0x10B20100U,
        0x5B3B4571U, 0x9508cf3eU, 0xfcd7bbaeU);


    _BlePluginClearScanFilter();
    _BlePluginAddScanServiceUuid(gattUUID);
    _BlePluginStartScan();
    BlePlugin::BleDeviceWatcher& watcher = BlePlugin::BleDeviceWatcher::GetInstance();

    while (true) {
        _BlePluginUpdateWatcher();

        debugDevices(watcher);
        if (_BlePluginScanGetDeviceLength() > 0) {
            break;
        }
        Sleep(500);
    }

    auto addr = _BlePluginScanGetDeviceAddr(0);
    _BlePluginConnectDevice(addr);

    byte data[7];
    data[0] = 0x01;
    data[1] = 0x01;
    data[2] = 0x01;
    data[3] = 100;
    data[4] = 0x02;
    data[5] = 0x01;
    data[6] = 100;
    bool isWrite = false;

    while (true) {
        _BlePluginUpdateDevicdeManger();

        if (!isWrite && _BlePluginGetConectDeviceNum() > 0 )
        {
            isWrite = true;
            _BlePluginWriteCharacteristicRequest(addr, serviceUUID,
                charaUUID, data, 7);
            break;
        }
    }
    ButtonSetSubscrive(addr);
    for (int i = 0; i < 10; ++i) {
        _BlePluginUpdateDevicdeManger();
        int deviceNum = _BlePluginGetConectDeviceNum();
        std::cout << "ButtonCheck " << deviceNum << std::endl;
        for (int j = 0; j < deviceNum; ++j) {
            int notificateNum = _BlePluginGetDeviceNotificateNum(addr);
            std::cout << "Notificate " << notificateNum << std::endl;
        }
        Sleep(100);
    }

}


int main()
{
    // init
    _BlePluginBleAdapterStatusRequest();

    int adapterStatus;
    while (true) {
        adapterStatus = _BlePluginBleAdapterUpdate();
        if (adapterStatus >= 0) {
            break;
        }
    }
    if (adapterStatus != 0) {
        std::cout << "Bluetooth adapter Error " << adapterStatus << std::endl;
        return 0;
    }
    while (true) {
        TestRun();
        _BlePluginDisconnectAllDevice();
        _BlePluginFinalize();
        Sleep(300);
    }
}
#endif
