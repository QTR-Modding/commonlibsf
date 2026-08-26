#pragma once

#include "RE/T/ThirdPersonState.h"

namespace RE
{
	class DialogueCameraState :
		public ThirdPersonState  // 000
	{
	public:
		SF_RTTI_VTABLE(DialogueCameraState);

		~DialogueCameraState() override;  // 00

		// members
		std::byte unk2A8[0x388 - 0x2A8];  // 2A8
	};
	static_assert(sizeof(DialogueCameraState) == 0x388);
}
