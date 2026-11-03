#pragma once

#include "RE/B/BSFixedString.h"

namespace RE
{
	class BSAnimationGraphEvent
	{
	public:
		std::byte     opaque00[0x8];  // 00
		BSFixedString tag;            // 08
		BSFixedString payload;        // 10
	};
	static_assert(offsetof(BSAnimationGraphEvent, tag) == 0x8);
	static_assert(offsetof(BSAnimationGraphEvent, payload) == 0x10);
	static_assert(sizeof(BSAnimationGraphEvent) == 0x18);
}
