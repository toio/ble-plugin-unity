#include "pch.h"
#include "BleDeviceObject.h"
#include "BleDeviceWatcher.h"
#include "BleDeviceManager.h"
#include "BluetoothAdapterChecker.h"
#include "UUidManager.h"
#include "Utility.h"
#include <windows.h>
#include "UnityInterface.h"
using namespace BlePlugin;

using namespace winrt::Windows::Foundation;

// DLL EntryPoint
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

DllExport void _BlePluginBleAdapterStatusRequest() {
    BluetoothAdapterChecker &checker = BluetoothAdapterChecker::GetInstance();
    checker.Request();
}
DllExport int _BlePluginBleAdapterUpdate() {
    BluetoothAdapterChecker& checker = BluetoothAdapterChecker::GetInstance();
    checker.Update();
    return static_cast<int>( checker.GetStatus() );
}

DllExport void _BlePluginFinalize() {
	BleDeviceWatcher& watcher = BleDeviceWatcher::GetInstance();
	watcher.Stop();
	watcher.ClearFilterServiceUUID();
	BleDeviceManager& manager = BleDeviceManager::GetInstance();
	manager.ResetAll();
}

DllExport void _BlePluginUpdateWatcher() {
	BleDeviceWatcher& watcher = BleDeviceWatcher::GetInstance();
	watcher.UpdateCache();
}


DllExport void _BlePluginUpdateDevicdeManger() {
	BleDeviceManager& manager = BleDeviceManager::GetInstance();
	manager.Update();
}

DllExport UuidHandle _BlePluginGetOrCreateUuidObject(uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4) {
	UuidManager& manager = UuidManager::GetInstance();
	auto ptr = manager.GetOrCreate(d1, d2, d3, d4);
	return ptr;
}
DllExport void _BlePluginConvertUuidUint128(UuidHandle ptr, void* out) {
	WinRtGuid* guid = reinterpret_cast<WinRtGuid*>(ptr);
	Utility::ConvertFromGUID(*guid, reinterpret_cast<uint32_t*>(out));
}


DllExport void _BlePluginAddScanServiceUuid(UuidHandle uuid) {
	auto guid = reinterpret_cast<WinRtGuid*>(uuid);
	BleDeviceWatcher& watcher = BleDeviceWatcher::GetInstance();
	watcher.AddServiceUUID(*guid);
}
DllExport void _BlePluginStartScan() {
	BleDeviceWatcher& watcher = BleDeviceWatcher::GetInstance();
	watcher.Start();
}
DllExport void _BlePluginStopScan() {
	BleDeviceWatcher& watcher = BleDeviceWatcher::GetInstance();
	watcher.Stop();
}

DllExport void _BlePluginClearScanFilter() {
	BleDeviceWatcher& watcher = BleDeviceWatcher::GetInstance();
	watcher.ClearFilterServiceUUID();
}

DllExport int _BlePluginScanGetDeviceLength() {
	BleDeviceWatcher& watcher = BleDeviceWatcher::GetInstance();
	return watcher.GetDeviceNum();
}

DllExport uint64_t _BlePluginScanGetDeviceAddr(int idx) {
	BleDeviceWatcher& watcher = BleDeviceWatcher::GetInstance();
	return watcher.GetAddr(idx);
}
const char* _BlePluginScanGetDeviceName(int idx) {
	BleDeviceWatcher& watcher = BleDeviceWatcher::GetInstance();
	return watcher.GetName(idx);
}

DllExport int32_t _BlePluginScanGetDeviceRssi(int idx) {
	BleDeviceWatcher& watcher = BleDeviceWatcher::GetInstance();
	return watcher.GetRssi(idx);
}

DllExport void _BlePluginScanCopyDeviceManifactureData(int idx, void* ptr, int max) {
	BleDeviceWatcher& watcher = BleDeviceWatcher::GetInstance();
}



DllExport DeviceHandle _BlePluginConnectDevice(uint64_t addr) {
	BleDeviceManager& manager = BleDeviceManager::GetInstance();
	BleDeviceObject *obj = manager.ConnectDevice(addr);
	return obj;
}
DllExport void _BlePluginDisconnectDevice(uint64_t addr) {
	BleDeviceManager& manager = BleDeviceManager::GetInstance();
	manager.DisconnectDevice(addr);
}

DllExport void _BlePluginDisconnectAllDevice() {
	BleDeviceManager& manager = BleDeviceManager::GetInstance();
	manager.DisconnectAll();
}

DllExport bool _BlePluginIsDeviceConnectedByAddr(uint64_t addr) {
	BleDeviceManager& manager = BleDeviceManager::GetInstance();
	BleDeviceObject* obj = manager.GetDeviceByAddr(addr);
	if (obj == nullptr) {
		return false;
	}
	return obj->IsConnected();
}
DllExport bool _BlePluginIsDeviceConnected(DeviceHandle devicePtr) {

	BleDeviceObject* obj = reinterpret_cast<BleDeviceObject*>(devicePtr);
	if (obj == nullptr) {
		return false;
	}
	return obj->IsConnected();
}
DllExport uint64_t _BlePluginDeviceGetAddr(DeviceHandle devicePtr) {
	BleDeviceObject* obj = reinterpret_cast<BleDeviceObject*>(devicePtr);
	if (obj == nullptr) {
		return 0;
	}
	return obj->GetAddr();
}


DllExport int _BlePluginGetConectDeviceNum() {
	BleDeviceManager& manager = BleDeviceManager::GetInstance();
	return manager.GetConnectedDeviceNum();
}
DllExport uint64_t _BlePluginGetConectDevicAddr(int idx) {
	BleDeviceManager& manager = BleDeviceManager::GetInstance();
	BleDeviceObject* obj = manager.GetConnectedDeviceByIndex(idx);
	if (obj != nullptr) {
		return obj->GetAddr();
	}
	return 0;
}

DllExport DeviceHandle _BlePluginGetConnectDevicePtr(int idx) {
	BleDeviceManager& manager = BleDeviceManager::GetInstance();
	BleDeviceObject* obj = manager.GetConnectedDeviceByIndex(idx);
	return obj;
}
DllExport DeviceHandle _BlePluginGetDevicePtrByAddr(uint64_t addr) {
	BleDeviceManager& manager = BleDeviceManager::GetInstance();
	BleDeviceObject* obj = manager.GetDeviceByAddr(addr);
	return obj;
}

DllExport int _BlePluginDeviceCharastricsNum(DeviceHandle devicePtr) {
	BleDeviceObject* deviceObj = reinterpret_cast<BleDeviceObject*>(devicePtr);
	if (deviceObj == nullptr) {
		return 0;
	}
	return deviceObj->GetCharastricsNum();
}
DllExport UuidHandle _BlePluginDeviceCharastricUuid(DeviceHandle devicePtr, int idx) {
	BleDeviceObject* deviceObj = reinterpret_cast<BleDeviceObject*>(devicePtr);
	if (deviceObj == nullptr) {
		return nullptr;
	}
	WinRtGuid guid = deviceObj->GetCharastrics(idx).Uuid();
	WinRtGuid *retval = UuidManager::GetInstance().GetOrCreate(guid);
	return retval;
}
DllExport UuidHandle _BlePluginDeviceCharastricServiceUuid(DeviceHandle devicePtr, int idx) {
	BleDeviceObject* deviceObj = reinterpret_cast<BleDeviceObject*>(devicePtr);
	if (deviceObj == nullptr) {
		return nullptr;
	}
	WinRtGuid guid = deviceObj->GetCharastrics(idx).Service().Uuid();
	WinRtGuid* retval = UuidManager::GetInstance().GetOrCreate(guid);
	return retval;
}


DllExport ReadRequestHandle _BlePluginReadCharacteristicRequest(uint64_t addr, UuidHandle serviceUuid, UuidHandle charaUuid) {
	BleDeviceManager& manager = BleDeviceManager::GetInstance();
	BleDeviceObject* deviceObj = manager.GetDeviceByAddr(addr);
	WinRtGuid* serviceUuidObj = reinterpret_cast<WinRtGuid*>(serviceUuid);
	WinRtGuid* charaUuidObj = reinterpret_cast<WinRtGuid*>(charaUuid);
	if (deviceObj == nullptr || serviceUuidObj == nullptr ||
		charaUuidObj == nullptr) {
		return nullptr;
	}
	auto ptr = deviceObj->ReadRequest(*serviceUuidObj, *charaUuidObj);
	return reinterpret_cast<void*>(ptr);
}

DllExport WriteRequestHandle _BlePluginWriteCharacteristicRequest(uint64_t addr, UuidHandle serviceUuid, UuidHandle charaUuid, void* data, int size) {

	BleDeviceManager& manager = BleDeviceManager::GetInstance();
	BleDeviceObject *deviceObj = manager.GetDeviceByAddr(addr);
	WinRtGuid* serviceUuidObj = reinterpret_cast<WinRtGuid*>(serviceUuid);
	WinRtGuid* charaUuidObj = reinterpret_cast<WinRtGuid*>(charaUuid);
	if (deviceObj == nullptr || serviceUuidObj == nullptr || 
		charaUuidObj == nullptr ) {
		return nullptr;
	}
	auto ptr = deviceObj->WriteRequest(*serviceUuidObj, *charaUuidObj,
		reinterpret_cast<uint8_t*>(data), size);
	return reinterpret_cast<void*>(ptr);

}
DllExport bool _BlePluginIsReadRequestComplete(ReadRequestHandle ptr) {
	auto operation = reinterpret_cast<WinRtAsyncOperation< WinRtGattReadResult>*>(ptr);
	if (operation == nullptr) {
		return true;
	}
	return (operation->Status() == AsyncStatus::Completed);
}
DllExport bool _BlePluginIsReadRequestError(ReadRequestHandle ptr) {
	auto operation = reinterpret_cast<WinRtAsyncOperation< WinRtGattReadResult>*>(ptr);
	if (operation == nullptr) {
		return true;
	}
	return (operation->Status() == AsyncStatus::Error);
}
DllExport int _BlePluginCopyReadRequestData(ReadRequestHandle ptr, void* data, int maxSize) {
	auto operation = reinterpret_cast<WinRtAsyncOperation< WinRtGattReadResult>*>(ptr);
	if (operation == nullptr) {
		return 0;
	}
	if (operation->Status() != AsyncStatus::Completed) {
		return 0;
	}
	if (operation->get().Status() != WinRtGattCommunicateState::Success) {
		return 0;
	}
	if (maxSize > NotificateData::MaxDataSize) {
		maxSize = NotificateData::MaxDataSize;
	}
	int size = operation->get().Value().Length();
	uint8_t* dest = reinterpret_cast<uint8_t*>(data);
	auto src = operation->get().Value().data();

	for (int i = 0; i < size && i < maxSize; ++i) {
		dest[i] = src[i];
	}
	return size;
}
DllExport void _BlePluginReleaseReadRequest(uint64_t deviceaddr, ReadRequestHandle ptr) {
	BleDeviceManager& manager = BleDeviceManager::GetInstance();
	BleDeviceObject* deviceObj = manager.GetDeviceByAddr(deviceaddr);
	if (deviceObj == nullptr) {
		return;
	}
	auto operation = reinterpret_cast<WinRtAsyncOperation< WinRtGattReadResult>*>(ptr);
	deviceObj->RemoveReadOperation(operation);
}

DllExport bool _BlePluginIsWriteRequestComplete(WriteRequestHandle ptr) {
	auto operation = reinterpret_cast<WinRtAsyncOperation< WinRtGattWriteResult>*>(ptr);
	if (operation == nullptr) {
		return true;
	}
	return (operation->Status() == AsyncStatus::Completed);
}
DllExport bool _BlePluginIsWriteRequestError(WriteRequestHandle ptr) {
	auto operation = reinterpret_cast<WinRtAsyncOperation< WinRtGattWriteResult>*>(ptr);
	if (operation == nullptr) {
		return false;
	}
	if (operation->Status() == AsyncStatus::Error) {
		return true;
	}
	if (operation->Status() == AsyncStatus::Completed && 
		operation->get().Status() != WinRtGattCommunicateState::Success) {
		return true;
	}
	return false;
}
DllExport void _BlePluginReleaseWriteRequest(uint64_t deviceaddr, WriteRequestHandle ptr) {
	BleDeviceManager& manager = BleDeviceManager::GetInstance();
	BleDeviceObject* deviceObj = manager.GetDeviceByAddr(deviceaddr);
	if (deviceObj == nullptr) {
		return;
	}
	auto operation = reinterpret_cast<WinRtAsyncOperation< WinRtGattWriteResult>*>(ptr);
	deviceObj->RemoveWriteOperation(operation);
}


// Notification
DllExport void _BlePluginSetNotificateRequest(uint64_t addr, UuidHandle serviceUuid, UuidHandle charaUuid, bool enable) {
	BleDeviceManager& manager = BleDeviceManager::GetInstance();
	BleDeviceObject* deviceObj = manager.GetDeviceByAddr(addr);
	WinRtGuid* serviceUuidObj = reinterpret_cast<WinRtGuid*>(serviceUuid);
	WinRtGuid* charaUuidObj = reinterpret_cast<WinRtGuid*>(charaUuid);
	if (deviceObj == nullptr || serviceUuidObj == nullptr ||
		charaUuidObj == nullptr) {
		return;
	}
	deviceObj->SetValueChangeNotification(*serviceUuidObj, *charaUuidObj, enable);
}

DllExport int _BlePluginGetDeviceNotificateNum(uint64_t addr) {
	BleDeviceManager& manager = BleDeviceManager::GetInstance();
	BleDeviceObject* deviceObj = manager.GetDeviceByAddr(addr);
	if (deviceObj == nullptr) {
		return 0;
	}
	return deviceObj->GetNofiticateNum();
}
DllExport int _BlePluginCopyDeviceNotificateData(uint64_t addr, int idx, void* ptr, int maxSize) {
	BleDeviceManager& manager = BleDeviceManager::GetInstance();
	BleDeviceObject* deviceObj = manager.GetDeviceByAddr(addr);
	if (deviceObj == nullptr) {
		return 0;
	}
	const NotificateData& notifyData = deviceObj->GetNotificateData(idx);
	const uint8_t* src = notifyData.GetData();
	int size = notifyData.GetSize();
	if (maxSize > NotificateData::MaxDataSize) {
		maxSize = NotificateData::MaxDataSize;
	}
	uint8_t* dest = reinterpret_cast<uint8_t*>(ptr);
	for (int i = 0; i < maxSize && i < size; ++i) {
		dest[i] = src[i];
	}
	return size;
}
	
DllExport UuidHandle _BlePluginGetDeviceNotificateServiceUuid(uint64_t addr, int idx) {
	BleDeviceManager& manager = BleDeviceManager::GetInstance();
	BleDeviceObject* deviceObj = manager.GetDeviceByAddr(addr);
	if (deviceObj == nullptr) {
		return nullptr;
	}
	const NotificateData& notifyData = deviceObj->GetNotificateData(idx);
	UuidManager &uuidMgr = UuidManager::GetInstance();	
	return uuidMgr.GetOrCreate( notifyData.GetServiceUuid() );
}

DllExport UuidHandle _BlePluginGetDeviceNotificateCharastricsUuid(uint64_t addr, int idx) {
	BleDeviceManager& manager = BleDeviceManager::GetInstance();
	BleDeviceObject* deviceObj = manager.GetDeviceByAddr(addr);
	if (deviceObj == nullptr) {
		return nullptr;
	}
	const NotificateData& notifyData = deviceObj->GetNotificateData(idx);
	UuidManager& uuidMgr = UuidManager::GetInstance();
	return uuidMgr.GetOrCreate(notifyData.GetCharastricsUuid());
}

