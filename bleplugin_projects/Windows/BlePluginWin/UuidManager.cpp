#include "UuidManager.h"
#include "Utility.h"

using namespace BlePlugin;

UuidManager UuidManager::s_instance;
UuidManager::UuidManager() {
}
WinRtGuid* UuidManager::GetOrCreate(uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4) {
	WinRtGuid guid = Utility::CreateGUID(d1, d2, d3, d4);
	return this->GetOrCreate(guid);
}

WinRtGuid* UuidManager::GetOrCreate(const WinRtGuid& guid) {
	for (auto it = m_cache.begin(); it != m_cache.end(); ++it) {
		if (guid == *it) {
			return &(*it);
		}
	}
	auto insertIt = (m_cache.insert(m_cache.begin(), guid));
	return &(*insertIt);
}


UuidManager& UuidManager::GetInstance() {
	return s_instance;
}

