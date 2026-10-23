#pragma once

#include "RE/I/IMovementInterface.h"
#include "RE/N/NiPoint.h"

namespace RE
{
	class IMovementSetRollTarget : public IMovementInterface
	{
	public:
		SF_RTTI_VTABLE(IMovementSetRollTarget);

		~IMovementSetRollTarget() override;  // 00

		// The target is an up vector in the movement frame, not an Euler roll angle.
		virtual void SetRollTarget(const NiPoint3& a_up) = 0;  // 01
	};
	static_assert(sizeof(IMovementSetRollTarget) == 0x8);
}
