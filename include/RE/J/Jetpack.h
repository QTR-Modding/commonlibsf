#pragma once

#include "RE/A/ActorValueOwner.h"
#include "RE/B/BSTEvent.h"
#include "RE/E/Events.h"

namespace RE
{
	class Actor;
	class ActorValueInfo;
	class ButtonEvent;

	// Runtime mappings exercised by the native boostpack implementation.
	namespace Jetpack
	{
		inline constexpr std::size_t kValueModifierEffectVTableIndex = 0;
		inline constexpr std::size_t kModifyActorValueVFunc = 0x1E;
		using ModifyActorValue_t = void (*)(void*, Actor*, float, ActorValueInfo*);

		inline constexpr std::size_t kJumpPressVTableIndex = 1;
		inline constexpr std::size_t kProcessJumpPressVFunc = 0x01;
		using ProcessJumpPress_t = BSEventNotifyControl (*)(
			void*,
			const PlayerControls::PlayerJumpPressEvent&,
			BSTEventSource<PlayerControls::PlayerJumpPressEvent>*);

		inline constexpr std::size_t kJumpHandlerVTableIndex = 0;
		inline constexpr std::size_t kOnButtonEventVFunc = 0x08;
		using OnButtonEvent_t = void (*)(void*, const ButtonEvent*);
	}
}
