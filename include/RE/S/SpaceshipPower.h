#pragma once

#include "RE/B/BSTArray.h"

namespace RE
{
	class TESObjectREFR;

	namespace Spaceship::Power
	{
		enum class System : std::uint32_t
		{
			Weapon,
			Engine,
			Shield,
			GravDrive
		};

		struct Component
		{
			System        system;
			std::uint32_t weaponGroup;
			std::int32_t  allocated;
			std::int32_t  bonus;
			std::int32_t  maximum;
			std::int32_t  damagePhysical;
			std::int32_t  damageEM;
		};

		struct Snapshot
		{
			std::int32_t        available{};
			std::int32_t        maximum{};
			BSTArray<Component> components;
		};

		// Call on the game thread with the player's ship. Weapon groups are zero-based.
		// The snapshot owns its values; component storage is not retained between calls.
		[[nodiscard]] bool Read(TESObjectREFR* a_ship, Snapshot& a_snapshot);

		// Changes one allocation pip (delta must be -1 or +1). Non-weapon systems
		// ignore weaponGroup. Native reactor, damage and manual-repair rules apply.
		// Call on the game thread with the player's ship. True means the allocation changed synchronously.
		[[nodiscard]] bool Change(TESObjectREFR* a_ship, System a_system, std::uint32_t a_weaponGroup, std::int32_t a_delta);
	}
}
