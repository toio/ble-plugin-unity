#pragma once
#include "pch.h"

#define DllExport  __declspec(dllexport)

extern "C" {
	typedef void* UuidHandle;
	typedef void* DeviceHandle;
	typedef void* WriteRequestHandle;
	typedef void* ReadRequestHandle;

	DllExport void _BlePluginFinalize();


	DllExport UuidHandle _BlePluginGetOrCreateUuidObject(uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4);
	DllExport void _BlePluginConvertUuidUint128(UuidHandle ptr, void* out);

	DllExport void _BlePluginUpdateWatcher();
	DllExport void _BlePluginUpdateDevicdeManger();

	DllExport void _BlePluginAddScanServiceUuid(UuidHandle uuid);
	DllExport void _BlePluginStartScan();
	DllExport void _BlePluginStopScan();
	DllExport void _BlePluginClearScanFilter();

	// Scan Data
	DllExport int _BlePluginScanGetDeviceLength();
	DllExport uint64_t _BlePluginScanGetDeviceAddr(int idx);
	DllExport const char* _BlePluginScanGetDeviceName(int idx);
	DllExport int32_t _BlePluginScanGetDeviceRssi(int idx);

	// Connect Dissconnect
	DllExport DeviceHandle _BlePluginConnectDevice(uint64_t addr);
	DllExport void _BlePluginDisconnectDevice(uint64_t addr);
	DllExport void _BlePluginDisconnectAllDevice();
	DllExport bool _BlePluginIsDeviceConnectedByAddr(uint64_t addr);
	DllExport bool _BlePluginIsDeviceConnected(DeviceHandle devicePtr);
	DllExport uint64_t _BlePluginDeviceGetAddr(DeviceHandle devicePtr);


	// GetDevice
	DllExport int _BlePluginGetConectDeviceNum();
	DllExport uint64_t _BlePluginGetConectDevicAddr(int idx);
	DllExport DeviceHandle _BlePluginGetConnectDevicePtr(int idx);
	DllExport DeviceHandle _BlePluginGetDevicePtrByAddr(uint64_t addr);
	DllExport int _BlePluginDeviceCharastricsNum(DeviceHandle devicePtr);
	DllExport UuidHandle _BlePluginDeviceCharastricUuid(DeviceHandle devicePtr, int idx);
	DllExport UuidHandle _BlePluginDeviceCharastricServiceUuid(DeviceHandle devicePtr, int idx);

	// Read/Write Request
	DllExport ReadRequestHandle _BlePluginReadCharacteristicRequest(uint64_t addr, UuidHandle serviceUuid, UuidHandle charaUuid);
	DllExport WriteRequestHandle _BlePluginWriteCharacteristicRequest(uint64_t addr, UuidHandle serviceUuid, UuidHandle charaUuid, void* data, int size);

	DllExport bool _BlePluginIsReadRequestComplete(ReadRequestHandle ptr);
	DllExport bool _BlePluginIsReadRequestError(ReadRequestHandle ptr);
	DllExport int _BlePluginCopyReadRequestData(ReadRequestHandle ptr, void* data, int maxSize);
	DllExport void _BlePluginReleaseReadRequest(uint64_t deviceaddr , ReadRequestHandle ptr);

	DllExport bool _BlePluginIsWriteRequestComplete(WriteRequestHandle ptr);
	DllExport bool _BlePluginIsWriteRequestError(WriteRequestHandle ptr);
	DllExport void _BlePluginReleaseWriteRequest(uint64_t deviceaddr, WriteRequestHandle ptr);

	// notificate
	DllExport void _BlePluginSetNotificateRequest(uint64_t addr, UuidHandle serviceUuid, UuidHandle charaUuid, bool enable);
	
	DllExport int _BlePluginGetDeviceNotificateNum(uint64_t addr);
	DllExport int _BlePluginCopyDeviceNotificateData(uint64_t addr, int idx, void* ptr, int maxSize);
	DllExport UuidHandle _BlePluginGetDeviceNotificateServiceUuid(uint64_t addr, int idx);
	DllExport UuidHandle _BlePluginGetDeviceNotificateCharastricsUuid(uint64_t addr, int idx);

}

