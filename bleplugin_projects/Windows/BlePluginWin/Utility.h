#pragma once

#include "pch.h"
namespace BlePlugin {
	class Utility {
	public:
		inline static WinRtGuid CreateGUID(uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4) {
			WinRtGuid guid;
			guid.Data1 = d1;
			guid.Data2 = (d2 & 0xffff0000)>>16;
			guid.Data3 = (d2 & 0xffff);

			for (int i = 0; i < 8; ++i) {
				int idx = i & 0x03;
				int shift = (3 - idx) * 8;
				int val = d3;
				if (i >= 4) {
					val = d4;
				}
				guid.Data4[i] = (((val & (0xff << shift)) >> shift) & 0xff);
			}
			return guid;
		}
		inline static void ConvertFromGUID(const WinRtGuid &uuid,
			uint32_t *data) {
			data[0] = uuid.Data1;
			data[1] = (uuid.Data2 << 16) | uuid.Data3;

			for (int i = 0; i < 2; ++i) {
				data[2+i] = 0;
				for (int j = 0; j < 4; ++j) {
					int idx = j + i *4;
					int shift = (3 - idx) * 8;
					data[2 + i] |= uuid.Data4[idx] << shift;
				}
			}
		}


#if defined(_DEBUG)
		inline static void DebugGuid(const WinRtGuid &src) {
			std::cout << std::hex << src.Data1 << "-" <<
				src.Data2 << "-" << src.Data3 << "-";
			for (int i = 0; i < 8; ++i) {
				std::cout << (int)(src.Data4[i]);
			}
		}
#endif


	};
}