#pragma once

#include "pch.h"
#include <time.h>
#include <mutex>

namespace BlePlugin {


	class BleDeviceWatcher {
	private:

		// デバイス情報
		struct DeviceInfo {
			static const int TimeoutSec = 1500;
//			std::string name;
			uint64_t addr;
			int rssi;
			clock_t lastFound;
			DeviceInfo() :
				/* name(""),*/
				addr(0), rssi(0), lastFound(0) 
			{
			}
			DeviceInfo(const DeviceInfo& src);
			DeviceInfo(const char* _name,
				uint64_t _addr,
				int _rssi);

			const DeviceInfo& operator =(const DeviceInfo& src);

			inline bool IsTimeout(clock_t current)const {
				return ((current - this->lastFound) >= TimeoutSec);
			}

		};
		// member
		std::map<uint64_t, DeviceInfo> m_DeviceMap;
		std::vector<DeviceInfo> m_cacheData;
		static BleDeviceWatcher s_instance;
		std::mutex mtx;

	public:
		static BleDeviceWatcher& GetInstance();

		void ClearFilterServiceUUID();
		void AddServiceUUID(const WinRtGuid &guid);
		void AddServiceUUID(uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4);

		void Start();
		void Stop();

		void UpdateCache();
		int GetDeviceNum()const;
		const char* GetName(int idx)const;
		uint64_t GetAddr(int idx)const;
		int GetRssi(int idx)const;

		void OnConnectDevice(uint64_t addr);

	private:

		BleDeviceWatcher();
		~BleDeviceWatcher();

		static void ReceiveCallBack(
			WinRtBleAdvertiseWatcher sender,
			WinRtBleAdvertiseRecieveEventArg args);

		void OnReceive(
			WinRtBleAdvertiseWatcher sender,
			WinRtBleAdvertiseRecieveEventArg args);

		WinRtBleAdvertiseWatcher m_watcher;
		WinRtBleAdvertiseFilter m_filer;


	};

}