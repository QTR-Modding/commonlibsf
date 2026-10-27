#include "RE/S/SpaceshipDocking.h"

namespace RE::Spaceship::Docking
{
	float GetTargetRangeAllowance(TESObjectREFR* a_target)
	{
		// HUD distance and vanilla docking range both dispatch this target-size virtual.
		using func_t = float (*)(TESObjectREFR*);
		const auto* table = *reinterpret_cast<std::uintptr_t**>(a_target);
		return reinterpret_cast<func_t>(table[0x5B0 / sizeof(std::uintptr_t)])(a_target);
	}

	TESObjectREFR* FindPort(TESObjectREFR* a_source, TESObjectREFR* a_target)
	{
		using func_t = decltype(&FindPort);
		static REL::Relocation<func_t> func{ ID::Spaceship::Docking::FindPort };
		return func(a_source, a_target);
	}

	bool BeginDock(TESObjectREFR* a_source, TESObjectREFR* a_target, TESObjectREFR* a_targetPort)
	{
		using func_t = decltype(&BeginDock);
		static REL::Relocation<func_t> func{ ID::Spaceship::Docking::BeginDock };
		return func(a_source, a_target, a_targetPort);
	}
}
