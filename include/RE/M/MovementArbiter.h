#pragma once

#include "RE/B/BSIntrusiveRefCounted.h"

namespace RE
{
	class IMovementState;

	class MovementArbiter : public BSIntrusiveRefCounted
	{
	public:
		SF_RTTI_VTABLE(MovementArbiter);

		virtual ~MovementArbiter();  // 00

		virtual void Unk_01();  // 01
		virtual void Unk_02();  // 02
		virtual void Unk_03();  // 03
		virtual void Unk_04();  // 04
		virtual void Unk_05();  // 05
		virtual void Unk_06();  // 06
		virtual void Unk_07();  // 07
		virtual void Unk_08();  // 08
		virtual void Unk_09();  // 09
		virtual void Unk_0A();  // 0A
		virtual void Unk_0B();  // 0B
		virtual void Unk_0C();  // 0C
		virtual void Unk_0D();  // 0D
		virtual void Unk_0E();  // 0E
		virtual void Unk_0F();  // 0F
		virtual void Unk_10();  // 10
		virtual void Unk_11();  // 11
		virtual void Unk_12();  // 12

		// Borrowed from the controller's active schema. Not reference-counted here.
		IMovementState* movementState;  // 10
	};
	static_assert(offsetof(MovementArbiter, movementState) == 0x10);
	static_assert(sizeof(MovementArbiter) == 0x18);
}
