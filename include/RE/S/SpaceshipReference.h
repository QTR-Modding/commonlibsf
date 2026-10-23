#pragma once

#include "RE/N/NiMatrix3.h"

namespace RE::Spaceship
{
	// Copy returned by the native body-pose query, not a persistent component pointer.
	// Body translation uses doubles and is distinct from raw REFR/cell-local coordinates.
	struct BodyPose
	{
		double    translation[3];      // 00
		std::byte unk18[0x08];         // 18
		NiMatrix3 rotation;            // 20
		bool      hasOwnDynamicState;  // 50 - false for containing-body/BodyDraw fallbacks
		std::byte unk51[0x0F];         // 51
	};
	static_assert(offsetof(BodyPose, rotation) == 0x20);
	static_assert(offsetof(BodyPose, hasOwnDynamicState) == 0x50);
	static_assert(sizeof(BodyPose) == 0x60);
}
