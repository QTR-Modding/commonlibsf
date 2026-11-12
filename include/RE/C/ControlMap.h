#pragma once

#include "RE/B/BSFixedString.h"

namespace RE
{
	class ControlMap
	{
	public:
		enum class Context : std::uint8_t
		{
			kGameplay = 0,
			kQuickContainer = 6,
			kQuickContainerA = 7,
			kQuickContainerX = 8,
			kQuickContainerY = 9,
			kQuickContainerB = 10
		};

		[[nodiscard]] static ControlMap* GetSingleton();

		void PushInputContext(Context a_context, const BSFixedString& a_owner);
		// Removes the last matching entry. An empty owner matches any owner.
		[[nodiscard]] bool PopInputContext(Context a_context, const BSFixedString& a_owner);
	};
}
