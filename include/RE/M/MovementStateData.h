#pragma once

#include "RE/N/NiPoint.h"

namespace RE
{
	// Input shared by IMovementSetState and IMovementQueryState.
	// State 2 treats vector as a point to face, not as a direction. The caller
	// supplies the movement frame; setting this data performs no frame conversion.
	struct MovementStateData
	{
		std::uint32_t state;   // 00
		NiPoint3      vector;  // 04
		float         unk10;   // 10
		float         unk14;   // 14
		float         unk18;   // 18
	};
	static_assert(offsetof(MovementStateData, vector) == 0x4);
	static_assert(sizeof(MovementStateData) == 0x1C);
}
