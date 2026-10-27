#pragma once

#include "RE/T/TESForm.h"
#include "RE/T/TESFullName.h"

namespace RE
{
	class Actor;

	class TESReactionForm :
		public BaseFormComponent  // 00
	{
	public:
		SF_RTTI(TESReactionForm);
	};

	class __declspec(novtable) TESFaction :
		public TESForm,         // 000
		public TESFullName,     // 038
		public TESReactionForm  // 040
	{
	public:
		SF_RTTI_VTABLE(TESFaction);
		SF_FORMTYPE(FACT);

		~TESFaction() override;  // 00

		// Faction reaction toward a valid actor; call on the game thread.
		[[nodiscard]] std::uint32_t GetFactionReaction(Actor* a_actor)
		{
			using func_t = std::uint32_t (*)(Actor*, TESFaction*);
			static REL::Relocation<func_t> func{ ID::TESFaction::GetFactionReaction };
			return func(a_actor, this);
		}
	};
	//static_assert(sizeof(TESFaction) == 0x158);
}
