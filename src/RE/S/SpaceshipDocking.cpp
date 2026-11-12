#include "RE/S/SpaceshipDocking.h"

#include "RE/T/TESObjectREFR.h"

namespace RE::Spaceship::Docking
{
	float GetTargetRangeAllowance(TESObjectREFR* a_target)
	{
		// HUD distance and vanilla docking range both dispatch this target-size virtual.
		return a_target->GetBoundRadius();
	}

	bool IsInRange(TESObjectREFR* a_source, TESObjectREFR* a_target)
	{
		using func_t = decltype(&IsInRange);
		static REL::Relocation<func_t> func{ ID::Spaceship::Docking::IsInRange };
		return func(a_source, a_target);
	}

	Availability GetAvailability(TESObjectREFR* a_source, TESObjectREFR* a_target)
	{
		using func_t = decltype(&GetAvailability);
		static REL::Relocation<func_t> func{ ID::Spaceship::Docking::GetAvailability };
		return func(a_source, a_target);
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

	bool BeginUndock(TESObjectREFR* a_ship, TESObjectREFR* a_otherShip)
	{
		using func_t = decltype(&BeginUndock);
		static REL::Relocation<func_t> func{ ID::Spaceship::Docking::BeginUndock };
		return func(a_ship, a_otherShip);
	}
}
