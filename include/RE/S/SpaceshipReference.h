#pragma once

#include "RE/N/NiMatrix3.h"

#include <array>

namespace RE::Spaceship
{
	// Copy returned by the native body-pose query, not a persistent component pointer.
	// Body translation uses doubles and is distinct from raw REFR/cell-local coordinates.
	struct BodyPose
	{
		std::array<double, 4> translation;  // 00
		NiMatrix3              rotation;            // 20
		bool                   hasOwnDynamicState;  // 50 - false for containing-body/BodyDraw fallbacks
		std::byte              unk51[0x0F];         // 51
	};
	static_assert(offsetof(BodyPose, rotation) == 0x20);
	static_assert(offsetof(BodyPose, hasOwnDynamicState) == 0x50);
	static_assert(sizeof(BodyPose) == 0x60);
}
