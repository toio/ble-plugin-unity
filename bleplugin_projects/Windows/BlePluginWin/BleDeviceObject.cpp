#include "BleDeviceObject.h"
#include "BleDeviceManager.h"
#include "Utility.h"


using namespace BlePlugin;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Devices::Bluetooth;

BleDeviceObject::BleDeviceObject(uint64_t addr) :
m_addr(addr), m_device(nullptr),m_connectState(EConnectState::None)
{
}

bool BleDeviceObject::IsConnected()const {
	return (m_connectState == EConnectState::GattServiceComplete);
}


void BleDeviceObject::ConnectRequest() {
	UpdateDisconectCheck();
	if (m_connectState == EConnectState::None) {
		m_connectAsync = BluetoothLEDevice::FromBluetoothAddressAsync(m_addr);
		m_connectState = EConnectState::Connecting;
	}
}
void BleDeviceObject::Disconnect() {
	for (auto it = m_services.begin(); it != m_services.end(); ++it) {
		it->Close();
	}
    if (m_device != nullptr) {
        m_device.Close();
    }
    this->ClearDeviceInfo();
}

void BleDeviceObject::Update() {
	// DeviceRequest
	switch (m_connectState) {
	case EConnectState::Connecting:
		if (m_connectAsync.Status() == AsyncStatus::Completed) {
			m_device = m_connectAsync.get();
			m_connectGattAsync = m_device.GetGattServicesAsync();
			this->m_connectState = EConnectState::GattServiceRequesting;
		}
		else if (m_connectAsync.Status() == AsyncStatus::Error) {
			Disconnect();
		}
		break;
	case EConnectState::GattServiceRequesting:
		if (m_connectGattAsync.Status() == AsyncStatus::Completed) {
			auto gattResult = m_connectGattAsync.get();
			this->SetupGattServices(gattResult);
			this->m_connectState = EConnectState::GattCharastricsRequesting;
		}
		else if (m_connectGattAsync.Status() == AsyncStatus::Error) {
			Disconnect();
		}
		break;
	case EConnectState::GattCharastricsRequesting:
		UpdateCharacterisc();
		break;
	}
	this->UpdateDisconectCheck();
	this->UpdateNotification();
}
void BleDeviceObject::UpdateCharacterisc() {
	bool hasError = false;
	for (auto it = m_charastricsRequests.begin(); it != m_charastricsRequests.end(); ) {
		if (it->Status() == AsyncStatus::Completed) {
			auto result = it->get();
			SetupCharacterisc(result);
			it = m_charastricsRequests.erase(it);
		}
		else if (it->Status() == AsyncStatus::Error) {
			hasError = true;
		}
		else {
			++it;
		}
	}
	if (hasError) {
		Disconnect();
		return;
	}
	if (m_charastricsRequests.size() == 0) {
		m_connectState = EConnectState::GattServiceComplete;
	}
}	

void BleDeviceObject::SetupCharacterisc(const WinRtBleCharacteristicsResult& result) {
	auto charastricses = result.Characteristics();
	int size = charastricses.Size();
	if (size == 0) {
		return;
	}
	auto serviceUUid = charastricses.GetAt(0).Service().Uuid();

	for (int i = 0; i < size; ++i) {
		auto ch = charastricses.GetAt(i);
		m_charastrictics.push_back(ch);
	}
}



void BleDeviceObject::SetupGattServices(const WinRtBleGattServiceResult& result) {
	this->m_services.clear();
	auto services = result.Services();
	int size = services.Size();
	for (int i = 0; i < size; ++i) {
		m_services.push_back( services.GetAt(i) );
	}

	this->m_charastricsRequests.clear();
	for (auto it = m_services.begin(); it != m_services.end(); ++it) {
		WinRtAsyncOperation<WinRtBleCharacteristicsResult> request
			=it->GetCharacteristicsAsync();
		WinRtGuid guid = it->Uuid();

		m_charastricsRequests.push_back(request);
	}
}

WinRtBleCharacteristic* BleDeviceObject::GetCharastric(const WinRtGuid& serviceUuid, const WinRtGuid& charastricsUuid) {
	for (auto it = m_charastrictics.begin();it != m_charastrictics.end(); ++it) {
		if (it->Uuid() == charastricsUuid &&
			it->Service().Uuid() == serviceUuid) {
			return &*it;
		}
	}
	return nullptr;
}

WinRtAsyncOperation< WinRtGattWriteResult>* BleDeviceObject::WriteRequest(const WinRtGuid& serviceUuid, const WinRtGuid& charastricsUuid,
	const uint8_t* src, int size) {
	WinRtBleCharacteristic* charastrics = this->GetCharastric(serviceUuid, charastricsUuid);
	if (charastrics == nullptr) {
		return nullptr;
	}
	auto buf = winrt::Windows::Storage::Streams::Buffer(size);
	buf.Length(size);
	uint8_t* ptr = buf.data();
	for (int i = 0; i < size; ++i) {
		*ptr = *src;
		++ptr;
		++src;
	}
	auto result = charastrics->WriteValueWithResultAsync(buf);
	auto it = m_writeRequest.insert(m_writeRequest.begin(), result);
	return &(*it);
}

WinRtAsyncOperation< WinRtGattReadResult>* BleDeviceObject::ReadRequest(const WinRtGuid& serviceUuid, const WinRtGuid& charastricsUuid) {
	WinRtBleCharacteristic* charastrics = this->GetCharastric(serviceUuid, charastricsUuid);
	if (charastrics == nullptr) {
		return nullptr;
	}
	auto result = charastrics->ReadValueAsync();

	auto it = m_readRequest.insert(m_readRequest.begin(),result);
	return &(*it);
}

void BleDeviceObject::RemoveWriteOperation(WinRtAsyncOperation< WinRtGattWriteResult>* operation) {
	for (auto it = m_writeRequest.begin(); it != m_writeRequest.end(); ++it) {
		if (&(*it) == operation) {
			m_writeRequest.erase(it);
			break;
		}
	}
}
void BleDeviceObject::RemoveReadOperation(WinRtAsyncOperation< WinRtGattReadResult>* operation) {
	for (auto it = m_readRequest.begin(); it != m_readRequest.end(); ++it) {
		if (&(*it) == operation) {
			m_readRequest.erase(it);
			break;
		}
	}
}


void BleDeviceObject::SetValueChangeNotification(const WinRtGuid& serviceUuid, const WinRtGuid& charastricsUuid, bool isnotificate) {
	WinRtBleCharacteristic* charastrics = this->GetCharastric(serviceUuid, charastricsUuid);
	if (charastrics == nullptr) {
		return;
	}
	if (isnotificate) {
		charastrics->WriteClientCharacteristicConfigurationDescriptorAsync(WinRtCharacteristicConfigValue::Notify);
		charastrics->ValueChanged(BleDeviceManager::Characteristic_ValueChanged);
	}
	else {
		charastrics->WriteClientCharacteristicConfigurationDescriptorAsync(WinRtCharacteristicConfigValue::None);
		charastrics->ValueChanged(nullptr);
	}
}

void BleDeviceObject::OnChangeValue(const WinRtGuid& serviceUuid, const WinRtGuid& charastricsUuid, uint8_t* data, int size) {
	NotificateData notificateData(serviceUuid, charastricsUuid, data, size);
	{
		std::lock_guard lock(m_notificateMutex);
		this->m_NotificateBuffer.push_back(notificateData);
	}
}

void BleDeviceObject::UpdateNotification() {
	m_NotificateResult.clear();
	std::lock_guard lock(m_notificateMutex);
	for (auto it = m_NotificateBuffer.begin(); it != m_NotificateBuffer.end(); ++it) {
		m_NotificateResult.push_back(*it);
	}
	m_NotificateBuffer.clear();
}

void BleDeviceObject::UpdateDisconectCheck() {
	if (this->m_connectState != EConnectState::GattServiceComplete) {
		return;
	}
	if( this->m_device.ConnectionStatus() != WinRtBleConnectStatus::Connected){
		ClearDeviceInfo();
        this->m_connectState = EConnectState::None;
    }
}
void BleDeviceObject::ClearDeviceInfo() {
	m_services.clear();
	m_charastrictics.clear();

	m_charastricsRequests.clear();

	m_readRequest.clear();
	m_writeRequest.clear();

	m_NotificateBuffer.clear();
	m_NotificateResult.clear();

    m_device = WinRtBleDevice(nullptr);
}
