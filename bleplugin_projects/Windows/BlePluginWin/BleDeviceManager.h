#pragma once
#include "pch.h"



namespace BlePlugin {
	class BleDeviceObject;
	class BleDeviceManager {
		static BleDeviceManager s_instance;
		std::vector <  BleDeviceObject*> m_devices;
		std::vector <  BleDeviceObject*> m_connectDevices;

	public:
		static BleDeviceManager& GetInstance();

		BleDeviceObject* ConnectDevice(uint64_t addr);
		BleDeviceObject* GetDeviceByAddr(uint64_t addr);
		void DisconnectDevice(uint64_t addr);
		void DisconnectAll();
		void ResetAll();

		int GetConnectedDeviceNum()const;
		BleDeviceObject* GetConnectedDeviceByIndex(int idx);
		void Update();

		static winrt::fire_and_forget Characteristic_ValueChanged(WinRtBleCharacteristic const&, WinRtBleValueChangedEventArgs args);


	};
}