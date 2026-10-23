#pragma once

#include "RE/I/IMovementInterface.h"
#include "RE/N/NiPoint.h"

namespace RE
{
	class IMovementDeltaStore : public IMovementInterface
	{
	public:
		SF_RTTI(IMovementDeltaStore);

		~IMovementDeltaStore() override;  // 00

		// Stores this frame's integrated increments in the movement pipeline.
		// The engine owns the interface and its update context; this call does not lock it.
		virtual void StoreDeltas(const NiPoint3& a_linear, const NiPoint3& a_angular) = 0;  // 01
	};
	static_assert(sizeof(IMovementDeltaStore) == 0x8);
}
