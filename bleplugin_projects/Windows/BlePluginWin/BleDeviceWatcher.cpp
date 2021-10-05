#include "BleDeviceWatcher.h"
#include "Utility.h"
#include <time.h>

using namespace BlePlugin;


BleDeviceWatcher BleDeviceWatcher::s_instance;
// DeviceInfo
BleDeviceWatcher::DeviceInfo::DeviceInfo(const char* _name,
	uint64_t _addr,int _rssi) : 
	//name(_name),
	addr(_addr), rssi(_rssi) {
	this->lastFound = clock();
}

BleDeviceWatcher::DeviceInfo::DeviceInfo(const BleDeviceWatcher::DeviceInfo& src)
	:
	//name(src.name),
	addr(src.addr),rssi(src.rssi),lastFound(src.lastFound)
{
}


const BleDeviceWatcher::DeviceInfo &
	BleDeviceWatcher::DeviceInfo::operator= (const BleDeviceWatcher::DeviceInfo &src)
{
//	this->name = src.name;
	this->addr = src.addr;
	this->rssi = src.rssi;
	this->lastFound = src.lastFound;
	return *this;
}



//@BleWatcher
BleDeviceWatcher& BleDeviceWatcher::GetInstance() {
	return s_instance;
}

void BleDeviceWatcher::ClearFilterServiceUUID() {
	m_filer.Advertisement().ServiceUuids().Clear();
}

void BleDeviceWatcher::AddServiceUUID(const WinRtGuid& guid) {
	m_filer.Advertisement().ServiceUuids().Append(guid);
}

void BleDeviceWatcher::AddServiceUUID(uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4) {
	auto guid = Utility::CreateGUID(d1, d2, d3, d4);
	m_filer.Advertisement().ServiceUuids().Append(guid);
}

void BleDeviceWatcher::Start() {
	m_watcher.Received(BleDeviceWatcher::ReceiveCallBack);
	m_watcher.AdvertisementFilter(m_filer);
	m_watcher.Start();
}
void BleDeviceWatcher::Stop() {
	m_watcher.Stop();
}

void BleDeviceWatcher::UpdateCache() {

	clock_t current = clock();
	std::lock_guard lock(mtx);
	m_cacheData.clear();
	for (auto it = m_DeviceMap.begin();
		it != m_DeviceMap.end(); ++it) {
		if (it->second.IsTimeout(current)) {
			continue;
		}
		m_cacheData.push_back(it->second);
	}
}

int BleDeviceWatcher::GetDeviceNum()const {
	return static_cast<int>(m_cacheData.size());
}

const char* BleDeviceWatcher::GetName(int idx)const {
	if (idx < 0 || idx >= m_cacheData.size()) {
		return nullptr;
	}
	return "";// m_cacheData.at(idx).name.c_str();
}
uint64_t BleDeviceWatcher::GetAddr(int idx)const {
	if (idx < 0 || idx >= m_cacheData.size()) {
		return 0;
	}
	return m_cacheData.at(idx).addr;
}

int BleDeviceWatcher::GetRssi(int idx)const {
	if (idx < 0 || idx >= m_cacheData.size()) {
		return 0;
	}
	return m_cacheData.at(idx).rssi;
}

void BleDeviceWatcher::OnConnectDevice(uint64_t addr) {
	m_DeviceMap.erase(addr);
}

BleDeviceWatcher::BleDeviceWatcher() {
}
BleDeviceWatcher::~BleDeviceWatcher() {
}

void BleDeviceWatcher::ReceiveCallBack(
	WinRtBleAdvertiseWatcher sender,
	WinRtBleAdvertiseRecieveEventArg args) {
	GetInstance().OnReceive(sender, args);
}

void BleDeviceWatcher::OnReceive(
	WinRtBleAdvertiseWatcher sender,
	WinRtBleAdvertiseRecieveEventArg args)
{
	std::lock_guard lock(mtx);
	int rssi = args.RawSignalStrengthInDBm();
	uint64_t addr = args.BluetoothAddress();
	auto advertisement = args.Advertisement();

	/* todo name And ManufactureData
	int dataSectionNum = advertisement.DataSections().Size();
	for (int i = 0; i < dataSectionNum; ++i) {
		auto dataSection = advertisement.DataSections().GetAt(i);
		uint8_t dataType = dataSection.DataType();
		int sectionLength = dataSection.Data().Length();
	}
	int manufactureSize = advertisement.ManufacturerData().Size();
	*/

//	auto name = advertisement.LocalName().c_str();
	const char* name = "";
	int nameSize = advertisement.LocalName().size();
	DeviceInfo deviceInfo( name, addr, rssi);
	m_DeviceMap[addr] = deviceInfo;
}

