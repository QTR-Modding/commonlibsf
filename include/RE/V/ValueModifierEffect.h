#pragma once

#include "RE/A/ActiveEffect.h"

namespace RE
{
	class Actor;
	class ActorValueInfo;

	class ValueModifierEffect :
		public ActiveEffect  // 000
	{
	public:
		SF_RTTI_VTABLE(ValueModifierEffect);

		~ValueModifierEffect() override;  // 00

		// override (ActiveEffect)
		bool  CheckCustomSkillUseConditions() const override;                     // 1C
		float GetCustomSkillUseMagnitudeMultiplier(float a_mult) const override;  // 1D

		// add
		virtual void ModifyActorValue(Actor* a_actor, float a_delta, ActorValueInfo* a_actorValue);  // 1E
		virtual bool ShouldModifyOnStart();                                                          // 1F
		virtual void ModifyOnStart();                                                                // 20
		virtual bool ShouldModifyOnUpdate() const;                                                   // 21
		virtual void ModifyOnUpdate(float a_delta);                                                  // 22
		virtual bool ShouldModifyOnFinish() const;                                                   // 23
		virtual void ModifyOnFinish(Actor* a_caster, Actor* a_target, float a_value);                // 24
		virtual bool Unk_25();                                                                       // 25

		// members
		ActorValueInfo* actorValue;  // 098
		float           value;       // 0A0
		float           unkA4;       // 0A4
		std::uint64_t   unkA8;       // 0A8
	};
	static_assert(sizeof(ValueModifierEffect) == 0xB0);
}
