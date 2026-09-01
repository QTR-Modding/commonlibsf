#pragma once

#include "RE/B/BSTEvent.h"

namespace RE
{
	struct BlurEvent
	{
	public:
		enum class Type : std::uint32_t
		{
			kIncrement = 0,
			kDecrement = 1
		};

		[[nodiscard]] static BSTEventSource<BlurEvent>* GetEventSource()
		{
			using func_t = BSTEventSource<BlurEvent>* (*)();
			static REL::Relocation<func_t> func{ ID::BlurEvent::GetEventSource };
			return func();
		}

		Type type;  // 00
	};
	static_assert(sizeof(BlurEvent) == 0x4);
}
