#pragma once

#include "RE/B/BSTEvent.h"
#include "RE/E/Events.h"
#include "RE/V/ValueModifierEffect.h"

namespace RE
{
	struct bhkNonSupportContactEvent;

	class JetpackEffect :
		public ValueModifierEffect,                                              // 000, VTABLE[0]
		public BSTEventSink<PlayerControls::PlayerJumpPressEvent>,               // 0B0, VTABLE[1]
		public BSTEventSink<PlayerControls::PlayerJumpReleaseEvent>,             // 0B8, VTABLE[4]
		public BSTEventSink<PlayerControls::PlayerZeroGSprintJustPressedEvent>,  // 0C0, VTABLE[5]
		public BSTEventSink<PlayerControls::PlayerZeroGSprintReleasedEvent>,     // 0C8, VTABLE[6]
		public BSTEventSink<PlayerControls::PlayerIronSightsStartEvent>,         // 0D0, VTABLE[7]
		public BSTEventSink<PlayerControls::PlayerIronSightsEndEvent>,           // 0D8, VTABLE[2]
		public BSTEventSink<bhkNonSupportContactEvent>                           // 0E0, VTABLE[3]
	{
	public:
		SF_RTTI_VTABLE(JetpackEffect);

		inline static constexpr std::size_t PRIMARY_VTABLE_INDEX = 0;
		inline static constexpr std::size_t JUMP_PRESS_VTABLE_INDEX = 1;
		inline static constexpr std::size_t IRON_SIGHTS_END_VTABLE_INDEX = 2;
		inline static constexpr std::size_t NON_SUPPORT_CONTACT_VTABLE_INDEX = 3;
		inline static constexpr std::size_t JUMP_RELEASE_VTABLE_INDEX = 4;
		inline static constexpr std::size_t ZERO_G_SPRINT_PRESS_VTABLE_INDEX = 5;
		inline static constexpr std::size_t ZERO_G_SPRINT_RELEASE_VTABLE_INDEX = 6;
		inline static constexpr std::size_t IRON_SIGHTS_START_VTABLE_INDEX = 7;

		~JetpackEffect() override;  // 00

		// override (BSTEventSink<PlayerControls::PlayerJumpPressEvent>, VTABLE[1])
		BSEventNotifyControl ProcessEvent(
			const PlayerControls::PlayerJumpPressEvent&           a_event,
			BSTEventSource<PlayerControls::PlayerJumpPressEvent>* a_source) override;  // 01

		// override (BSTEventSink<PlayerControls::PlayerJumpReleaseEvent>, VTABLE[4])
		BSEventNotifyControl ProcessEvent(
			const PlayerControls::PlayerJumpReleaseEvent&           a_event,
			BSTEventSource<PlayerControls::PlayerJumpReleaseEvent>* a_source) override;  // 01

		// override (BSTEventSink<PlayerControls::PlayerZeroGSprintJustPressedEvent>, VTABLE[5])
		BSEventNotifyControl ProcessEvent(
			const PlayerControls::PlayerZeroGSprintJustPressedEvent&           a_event,
			BSTEventSource<PlayerControls::PlayerZeroGSprintJustPressedEvent>* a_source) override;  // 01

		// override (BSTEventSink<PlayerControls::PlayerZeroGSprintReleasedEvent>, VTABLE[6])
		BSEventNotifyControl ProcessEvent(
			const PlayerControls::PlayerZeroGSprintReleasedEvent&           a_event,
			BSTEventSource<PlayerControls::PlayerZeroGSprintReleasedEvent>* a_source) override;  // 01

		// override (BSTEventSink<PlayerControls::PlayerIronSightsStartEvent>, VTABLE[7])
		BSEventNotifyControl ProcessEvent(
			const PlayerControls::PlayerIronSightsStartEvent&           a_event,
			BSTEventSource<PlayerControls::PlayerIronSightsStartEvent>* a_source) override;  // 01

		// override (BSTEventSink<PlayerControls::PlayerIronSightsEndEvent>, VTABLE[2])
		BSEventNotifyControl ProcessEvent(
			const PlayerControls::PlayerIronSightsEndEvent&           a_event,
			BSTEventSource<PlayerControls::PlayerIronSightsEndEvent>* a_source) override;  // 01

		// override (BSTEventSink<bhkNonSupportContactEvent>, VTABLE[3])
		BSEventNotifyControl ProcessEvent(
			const bhkNonSupportContactEvent&           a_event,
			BSTEventSource<bhkNonSupportContactEvent>* a_source) override;  // 01

		// members
		std::byte unk0E8[0x50];  // 0E8
	};
	static_assert(sizeof(JetpackEffect) == 0x138);
}
