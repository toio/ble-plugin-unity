#pragma once

#include "pch.h"

namespace BlePlugin {
	class NotificateData {
	public:
		// 上限は 22byte…
		static const int MaxDataSize = 22;
	private:
		WinRtGuid service;
		WinRtGuid charastrics;
		uint8_t data[MaxDataSize];
		int size;
	private:
		void SetData(const uint8_t* src, int length) {
			for (int i = 0; i < length && i < sizeof(data); ++i) {
				data[i] = src[i];
			}
		}
	public:
		NotificateData(const WinRtGuid &_service,
			const WinRtGuid& _charastrics,uint8_t* _data,int _size) :
			service(_service), charastrics(_charastrics),size(_size),data()
		{
			SetData(_data, _size);
		}
		NotificateData(const NotificateData& src) :
			service (src.service),charastrics(src.charastrics),
			size(src.size), data()
		{
			SetData(src.data, src.size);
		}
		NotificateData& operator =(const NotificateData& src) {
			this->service = src.service;
			this->charastrics = src.charastrics;
			SetData(src.data, src.size);
			return *this;
		}
		inline const WinRtGuid& GetServiceUuid()const {
			return service;
		}
		inline const WinRtGuid& GetCharastricsUuid()const {
			return charastrics;
		}
		inline const uint8_t* GetData()const {
			return data;
		}
		inline int GetSize()const {
			return size;
		}

	};

	class BleDeviceObject {
	private:
		enum class EConnectState {
			None = 0,
			Connecting = 1,
			GattServiceRequesting = 2,
			GattCharastricsRequesting = 3,
			GattServiceComplete = 4,
		};


		WinRtAsyncOperation<WinRtBleDevice> m_connectAsync;
		WinRtAsyncOperation<WinRtBleGattServiceResult> m_connectGattAsync;

		uint64_t m_addr;
		WinRtBleDevice m_device;

		std::vector<WinRtBleGattService> m_services;
		std::vector<WinRtBleCharacteristic> m_charastrictics;

		std::vector<WinRtAsyncOperation<WinRtBleCharacteristicsResult> > m_charastricsRequests;
		EConnectState m_connectState;

		std::list< WinRtAsyncOperation< WinRtGattReadResult> > m_readRequest;
		std::list< WinRtAsyncOperation< WinRtGattWriteResult> > m_writeRequest;
		std::vector< NotificateData> m_NotificateBuffer;
		std::vector< NotificateData> m_NotificateResult;
		std::mutex m_notificateMutex;

	public:
		BleDeviceObject(uint64_t addr);

		bool IsConnected()const;
		void ConnectRequest();
		void Disconnect();
		void Update();

		WinRtAsyncOperation< WinRtGattWriteResult>* WriteRequest(const WinRtGuid& serviceUuid, const WinRtGuid& charastricsUuid,
			const uint8_t* src, int size);
		WinRtAsyncOperation< WinRtGattReadResult>* ReadRequest(const WinRtGuid& serviceUuid, const WinRtGuid& charastricsUuid);

		void RemoveWriteOperation(WinRtAsyncOperation< WinRtGattWriteResult>* operation);
		void RemoveReadOperation(WinRtAsyncOperation< WinRtGattReadResult>* operation);

		void SetValueChangeNotification(const WinRtGuid& serviceUuid, const WinRtGuid& charastricsUuid,bool isnotificate);
		void OnChangeValue(const WinRtGuid& serviceUuid, const WinRtGuid& charastricsUuid,uint8_t *data, int length);

		int GetNofiticateNum()const {
			return static_cast<int>(m_NotificateResult.size());
		}
		const NotificateData& GetNotificateData(int idx)const {
			return m_NotificateResult.at(idx);
		}

		inline uint64_t GetAddr()const {
			return this->m_addr;
		}

		inline int GetCharastricsNum()const {
			return static_cast<int>( this->m_charastrictics.size());
		}
		const WinRtBleCharacteristic& GetCharastrics(int idx) const {
			return m_charastrictics.at(idx);
		}
	private:
		WinRtBleCharacteristic* GetCharastric(const WinRtGuid& serviceUuid, const WinRtGuid& charastricsUuid);
		void SetupGattServices(const WinRtBleGattServiceResult& result);
		void UpdateCharacterisc();
		void SetupCharacterisc(const WinRtBleCharacteristicsResult& result);

		void UpdateNotification();
		void UpdateDisconectCheck();
		void ClearDeviceInfo();
	};
}
