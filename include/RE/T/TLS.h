#pragma once

#include "REX/W32/NT.h"

namespace RE
{
	struct TLS
	{
		[[nodiscard]] static TLS* GetSingleton()
		{
			return *static_cast<TLS**>(REX::W32::NtCurrentTeb()->threadLocalStoragePointer);
		}

		// members
		std::byte     pad000[0x1D0];          // 000
		bool          logConditionFunctions;  // 1D0
		std::byte     pad1D1[0x93];           // 1D1
		std::uint32_t logicalThreadID;        // 264
	};
	static_assert(offsetof(TLS, logicalThreadID) == 0x264);
}
