#pragma once

#include "RE/B/BSTArray.h"
#include "RE/N/NiMatrix3.h"
#include "RE/N/NiQuaternion.h"

namespace RE
{
	class TESIdleForm;
	class TESObjectREFR;

	namespace Spaceship::Docking
	{
		// Call these native entry points on the game thread with valid, non-null references.
		enum class Availability : std::uint8_t
		{
			kEligible = 0,
			kRestricted = 1,
			kUnavailable = 2
		};

		struct StartPose
		{
			NiQuaternion rotation;
			NiPoint3     translation;
		};

		struct InteractionEntry
		{
			NiQuaternion  rotation;     // 00 - relative to the target ship
			NiPoint3      translation;  // 10
			std::uint32_t unk1C;        // 1C
			TESIdleForm*  idle;         // 20 - borrowed
			std::byte     unk28[0x18];  // 28
		};
		static_assert(offsetof(InteractionEntry, translation) == 0x10);
		static_assert(offsetof(InteractionEntry, idle) == 0x20);
		static_assert(sizeof(InteractionEntry) == 0x40);

		struct InteractionGroup
		{
			std::uint32_t                  index;    // 00
			std::uint32_t                  unk04;    // 04
			BSScrapArray<InteractionEntry> entries;  // 08 - owns its scratch allocation
		};
		static_assert(offsetof(InteractionGroup, entries) == 0x08);
		static_assert(sizeof(InteractionGroup) == 0x20);

		// Native range check: body separation against target radius plus the live docking limit.
		// Asymmetric; missing body-draw data returns false. Does not check approach clearance.
		[[nodiscard]] bool IsInRange(TESObjectREFR* a_source, TESObjectREFR* a_target);

		// Native HUD candidate classification. The HUD can apply additional restrictions.
		// This does not start or reserve a docking action; kRestricted supplies no reason code.
		[[nodiscard]] Availability GetAvailability(TESObjectREFR* a_source, TESObjectREFR* a_target);

		// Selects a compatible target port for the source ship. The returned reference is borrowed.
		[[nodiscard]] TESObjectREFR* FindPort(TESObjectREFR* a_source, TESObjectREFR* a_target);

		// Queries the start locations using the same full-rotation/placement options as vanilla Dock.
		// Replaces a_out; groups/entries own their scratch storage, while idle form pointers are borrowed.
		// Requires a valid source ship and a port returned by FindPort. Does not move either reference.
		[[nodiscard]] bool GetStartLocations(TESObjectREFR* a_source, TESObjectREFR* a_targetPort, BSScrapArray<InteractionGroup>& a_out);

		// Copies vanilla's initial interaction pose relative to the target ship (the docking parent).
		// Compose with the target's current body transform before flight; this is not an absolute pose.
		// Does not move the ship; failure leaves a_out unchanged.
		// Requires a valid source ship and a port returned by FindPort for that ship/target pair.
		[[nodiscard]] bool GetStartPose(TESObjectREFR* a_source, TESObjectREFR* a_targetPort, StartPose& a_out);

		// Runs vanilla's final docking action with valid ship references and their selected port.
		// True means the action was accepted, not completed. The player-associated branch can
		// reposition the ship and start the vanilla docking camera; this wrapper does not suppress it.
		// A false result can already have submitted movement; do not use this call as a query.
		[[nodiscard]] bool BeginDock(TESObjectREFR* a_source, TESObjectREFR* a_target, TESObjectREFR* a_targetPort);

		// Begins vanilla undocking for a connected pair; the engine normalizes child/parent order.
		// True means accepted, not finished. Vanilla camera behavior is preserved; departure flight
		// is a separate operation. Call with valid references on the game thread.
		[[nodiscard]] bool BeginUndock(TESObjectREFR* a_ship, TESObjectREFR* a_otherShip);
	}
}
