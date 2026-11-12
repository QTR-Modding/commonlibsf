#pragma once

#include "RE/I/IMovementInterface.h"
#include "RE/M/MovementStateData.h"

namespace RE
{
	class IMovementSetState : public IMovementInterface
	{
	public:
		SF_RTTI_VTABLE(IMovementSetState);

		~IMovementSetState() override;  // 00

		virtual void SetStateWithParameters(const MovementStateData& a_state, float a_unk1, float a_unk2) = 0;  // 01
		virtual void SetState(const MovementStateData& a_state) = 0;                                            // 02
	};
	static_assert(sizeof(IMovementSetState) == 0x8);
}
