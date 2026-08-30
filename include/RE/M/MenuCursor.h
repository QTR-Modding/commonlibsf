#pragma once

#include "RE/IDs.h"

namespace RE
{
	// Singleton mapping and partial field overlay adapted from ozooma10/commonlibsf
	// commit c6828bf744b215562370cee607e32efc21444406. Do not construct or use it for
	// virtual dispatch.
	class MenuCursor
	{
	public:
		[[nodiscard]] static MenuCursor* GetSingleton()
		{
			static REL::Relocation<MenuCursor**> singleton{ ID::MenuCursor::Singleton };
			return *singleton;
		}

		// members
		std::byte     pad000[0x90];        // 000
		std::uint32_t freeCursorRefCount;  // 090
		bool          warpRequested;       // 094
		bool          cursorActive;        // 095
	};
	static_assert(offsetof(MenuCursor, freeCursorRefCount) == 0x90);
	static_assert(offsetof(MenuCursor, warpRequested) == 0x94);
	static_assert(offsetof(MenuCursor, cursorActive) == 0x95);
}
