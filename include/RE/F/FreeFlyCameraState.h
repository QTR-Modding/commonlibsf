#pragma once

#include "RE/T/TESCameraState.h"

namespace RE
{
	class FreeFlyCameraState :
		public TESCameraState  // 00
	{
	public:
		SF_RTTI_VTABLE(FreeFlyCameraState);

		~FreeFlyCameraState() override;  // 00

		// members
		std::byte unk058[0xB5 - 0x58];  // 058
		// Negates mouse and right-stick Y during pitch integration.
		bool      invertY;               // 0B5
		std::byte pad0B6[0x2];           // 0B6
	};
	static_assert(sizeof(FreeFlyCameraState) == 0xB8);
	static_assert(offsetof(FreeFlyCameraState, invertY) == 0xB5);
}
