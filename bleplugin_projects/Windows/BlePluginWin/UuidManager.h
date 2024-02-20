#pragma once

#include "pch.h"
#include <vector>
#include <list>

namespace BlePlugin {
	class UuidManager {
	private:
		std::list<WinRtGuid> m_cache;
		static UuidManager s_instance;
		UuidManager();
	public:
		WinRtGuid* GetOrCreate(uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4);
		WinRtGuid* GetOrCreate(const WinRtGuid &guid);
		static UuidManager &GetInstance();

	};
}