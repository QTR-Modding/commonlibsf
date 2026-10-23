#pragma once

#include "RE/B/BSFixedString.h"
#include "RE/B/BSIntrusiveRefCounted.h"

namespace RE
{
	class IMovementState;
	struct IPipelineStageInterface;

	class MovementAgent : public BSIntrusiveRefCounted
	{
	public:
		SF_RTTI_VTABLE(MovementAgent);

		virtual ~MovementAgent();  // 00

		virtual void                     Unk_01();                                           // 01
		virtual void                     Unk_02();                                           // 02
		virtual IPipelineStageInterface* GetPipelineStage(const BSFixedString& a_name) = 0;  // 03
		virtual void                     Unk_04();                                           // 04
		virtual void                     Unk_05();                                           // 05
		virtual void                     Unk_06();                                           // 06
		virtual void                     Unk_07();                                           // 07
		virtual void                     Unk_08();                                           // 08
		virtual void                     Unk_09();                                           // 09
		virtual void                     Unk_0A();                                           // 0A
		virtual void                     Unk_0B();                                           // 0B
		virtual void                     Unk_0C();                                           // 0C
		virtual void                     Unk_0D();                                           // 0D

		// members
		IMovementState* movementState;  // 10 - borrowed from the controller's active schema
	};
	static_assert(offsetof(MovementAgent, movementState) == 0x10);
	static_assert(sizeof(MovementAgent) == 0x18);
}
