#pragma once

#include "RE/I/IMovementInterface.h"
#include "RE/M/MovementStateData.h"

namespace RE
{
	class IMovementQueryState : public IMovementInterface
	{
	public:
		SF_RTTI_VTABLE(IMovementQueryState);

		~IMovementQueryState() override;  // 00

		// Borrowed state; the returned reference does not retain its owner or lock it.
		virtual const MovementStateData& GetState() const = 0;  // 01
		virtual void                     Unk_02();              // 02
	};
	static_assert(sizeof(IMovementQueryState) == 0x8);
}
