#pragma once

#include "RE/B/BSIntrusiveRefCounted.h"

namespace RE
{
	class ActiveEffect :
		public BSIntrusiveRefCounted  // 08
	{
	public:
		SF_RTTI_VTABLE(ActiveEffect);

		virtual ~ActiveEffect();  // 00

		// add
		virtual void  Unk_01();                                                  // 01
		virtual void  Unk_02();                                                  // 02
		virtual void  Unk_03();                                                  // 03
		virtual void  Unk_04();                                                  // 04
		virtual void  Unk_05();                                                  // 05
		virtual void  Unk_06();                                                  // 06
		virtual bool  Unk_07();                                                  // 07
		virtual bool  Unk_08();                                                  // 08
		virtual bool  Unk_09();                                                  // 09
		virtual void  Unk_0A();                                                  // 0A
		virtual void  Unk_0B();                                                  // 0B
		virtual void  Unk_0C();                                                  // 0C
		virtual void  Unk_0D();                                                  // 0D
		virtual void  Unk_0E();                                                  // 0E
		virtual void  Unk_0F();                                                  // 0F
		virtual void  Unk_10();                                                  // 10
		virtual void  Unk_11();                                                  // 11
		virtual void  Unk_12();                                                  // 12
		virtual void  Unk_13();                                                  // 13
		virtual bool  Unk_14();                                                  // 14
		virtual bool  Unk_15();                                                  // 15
		virtual void  Unk_16();                                                  // 16
		virtual void  Unk_17();                                                  // 17
		virtual void  Unk_18();                                                  // 18
		virtual void  Unk_19();                                                  // 19
		virtual void  Unk_1A();                                                  // 1A
		virtual void  Unk_1B();                                                  // 1B
		virtual bool  CheckCustomSkillUseConditions() const;                     // 1C
		virtual float GetCustomSkillUseMagnitudeMultiplier(float a_mult) const;  // 1D

		// members
		std::byte unk0C[0x8C];  // 0C
	};
	static_assert(sizeof(ActiveEffect) == 0x98);
}
