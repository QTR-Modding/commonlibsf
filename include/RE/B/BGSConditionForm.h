#pragma once

#include "RE/T/TESCondition.h"
#include "RE/T/TESForm.h"

namespace RE
{
	namespace BSScript
	{
		class IVirtualMachine;
	}

	class TESQuest;
	class TESPackage;
	class TESObjectREFR;

	class BGSConditionForm : public TESForm
	{
	public:
		SF_RTTI_VTABLE(BGSConditionForm);
		SF_FORMTYPE(CNDF);

		~BGSConditionForm() override;  // 00

		// This invokes condition functions and must only be called on the game thread.
		[[nodiscard]] bool IsTrue(TESObjectREFR* a_actionRef, TESObjectREFR* a_targetRef)
		{
			using func_t = bool (*)(BSScript::IVirtualMachine*, std::uint32_t, BGSConditionForm*, TESObjectREFR*, TESObjectREFR*);
			static REL::Relocation<func_t> func{ ID::BGSConditionForm::IsTrue };
			return func(nullptr, 0, this, a_actionRef, a_targetRef);
		}

		// members
		TESCondition  conditions;  // 30
		TESQuest*     quest;       // 40
		TESPackage*   package;     // 48
		std::uint32_t unk50;       // 50
		std::uint8_t  unk54;       // 54
	};
	static_assert(sizeof(BGSConditionForm) == 0x60);
}
