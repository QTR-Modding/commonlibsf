#pragma once

#include "RE/I/IMovementInterface.h"

namespace RE
{
	class IMovementQueryPathFollowing : public IMovementInterface
	{
	public:
		SF_RTTI_VTABLE(IMovementQueryPathFollowing);

		~IMovementQueryPathFollowing() override;  // 00

		// Tests absence of the current path, not completion of a pending request.
		virtual bool HasNoPath() const = 0;  // 01

		// The remaining virtual functions are not yet mapped.
	};
	static_assert(sizeof(IMovementQueryPathFollowing) == 0x8);
}
