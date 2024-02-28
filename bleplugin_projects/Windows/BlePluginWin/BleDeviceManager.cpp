#include "BleDeviceManager.h"
#include "BleDeviceObject.h"

using namespace BlePlugin;


BleDeviceManager BleDeviceManager::s_instance;
BleDeviceManager& BleDeviceManager::GetInstance() {
	return s_instance;
}

BleDeviceObject* BleDeviceManager::ConnectDevice(uint64_t addr) {

	BleDeviceObject* deviceObj = this->GetDeviceByAddr(addr);
	if (deviceObj == nullptr) {
		deviceObj = new BleDeviceObject(addr);
		m_devices.push_back(deviceObj);
	}
	deviceObj->ConnectRequest();
	return nullptr;
}
BleDeviceObject* BleDeviceManager::GetDeviceByAddr(uint64_t addr) {
	for (auto it = m_devices.begin(); it != m_devices.end(); ++it) {
		BleDeviceObject* devicePtr = (*it);
		if (devicePtr  == nullptr) {
			continue;
		}
		if (devicePtr->GetAddr() == addr) {
			return devicePtr;
		}
	}
	return nullptr;
}
void BleDeviceManager::DisconnectDevice(uint64_t addr) {
	BleDeviceObject* deviceObj = this->GetDeviceByAddr(addr);
	if (deviceObj != nullptr) {
		deviceObj->Disconnect();
	}
}


void BleDeviceManager::DisconnectAll() {
	for (auto it = m_devices.begin(); it != m_devices.end(); ++it) {
		BleDeviceObject* deviceObj = *it;
		if (deviceObj == nullptr) {
			continue;
		}
		if (deviceObj->IsConnected()) {
			deviceObj->Disconnect();
		}
	}
}
void BleDeviceManager::ResetAll() {
	this->DisconnectAll();
	m_connectDevices.clear();
    // clear all device memory
    for (auto it = m_devices.begin(); it != m_devices.end(); ++it) {
        BleDeviceObject* devicePtr = (*it);
        if (devicePtr == nullptr) {
            continue;
        }
        delete devicePtr;
    }
	m_devices.clear();
}


int BleDeviceManager::GetConnectedDeviceNum()const {
	return static_cast<int>( m_connectDevices.size() );
}

BleDeviceObject* BleDeviceManager::GetConnectedDeviceByIndex(int idx) {
	return m_connectDevices.at(idx);
}

void BleDeviceManager::Update() {
	m_connectDevices.clear();
	for (auto it = m_devices.begin(); it != m_devices.end(); ++it) {
		BleDeviceObject* deviceObj = *it;
		if (deviceObj == nullptr) {
			continue;
		}
		deviceObj->Update();
		if (deviceObj->IsConnected()) {
			m_connectDevices.push_back(deviceObj);
		}
	}
}


winrt::fire_and_forget BleDeviceManager::Characteristic_ValueChanged(WinRtBleCharacteristic const& charastrics, WinRtBleValueChangedEventArgs args) {
	winrt::fire_and_forget ret;
	uint64_t addr = charastrics.Service().Device().BluetoothAddress();
	BleDeviceObject* obj = BleDeviceManager::GetInstance().GetDeviceByAddr(addr);
	if (obj == nullptr) {
		return ret;
	}
	WinRtGuid serviceUuid = charastrics.Service().Uuid();
	WinRtGuid charastricsUuid = charastrics.Uuid();
	uint8_t* data = args.CharacteristicValue().data();
	int length = args.CharacteristicValue().Length();
	obj->OnChangeValue(serviceUuid, charastricsUuid, data, length);
	return ret;
}
