#pragma once

#include "RE/I/IMovementDeltaStore.h"
#include "RE/I/IMovementQueryDeltas.h"
#include "RE/M/MovementArbiter.h"
#include "RE/N/NiPoint.h"

namespace RE
{
	// Engine-owned snapshot of the movement update. Access in the engine's
	// movement-update context; the stored fields and delta interfaces do not lock.
	class MovementPostUpdateArbiter :
		public MovementArbiter,        // 00
		private IMovementQueryDeltas,  // 18
		private IMovementDeltaStore    // 20
	{
	public:
		SF_RTTI_VTABLE(MovementPostUpdateArbiter);

		~MovementPostUpdateArbiter() override;

		// Borrowed interfaces. Their lifetime is bounded by this engine object.
		[[nodiscard]] IMovementQueryDeltas*       GetQueryDeltas() noexcept { return this; }
		[[nodiscard]] const IMovementQueryDeltas* GetQueryDeltas() const noexcept { return this; }
		[[nodiscard]] IMovementDeltaStore*        GetDeltaStore() noexcept { return this; }
		[[nodiscard]] const IMovementDeltaStore*  GetDeltaStore() const noexcept { return this; }

		std::byte                                  unk28[0x8];              // 28
		IMovementQueryDeltas::UpdateData           updateData;              // 30
		std::byte                                  unk74[0xC];              // 74
		IMovementQueryDeltas::CorrectionVelocities correctionVelocities;    // 80
		NiPoint3                                   startPosition;           // 98
		NiPoint3                                   startAngles;             // A4
		NiPoint3                                   previousRotationOffset;  // B0
		float                                      deltaTime;               // BC
		std::uint8_t                               frameState;              // C0
		std::byte                                  unkC1[0x3];              // C1
		NiPoint3                                   storedLinear;            // C4
		NiPoint3                                   storedAngular;           // D0
		NiPoint3                                   previousStoredLinear;    // DC
		bool                                       hasStoredDeltas;         // E8
		bool                                       hadStoredDeltas;         // E9
		std::byte                                  unkEA[0x6];              // EA
		// Retained by the engine. Register/unregister through the native pipeline.
		void*     interfaceRegistry;  // F0
		std::byte unkF8[0x8];         // F8
	};
	static_assert(offsetof(MovementPostUpdateArbiter, updateData) == 0x30);
	static_assert(offsetof(MovementPostUpdateArbiter, correctionVelocities) == 0x80);
	static_assert(offsetof(MovementPostUpdateArbiter, startPosition) == 0x98);
	static_assert(offsetof(MovementPostUpdateArbiter, previousRotationOffset) == 0xB0);
	static_assert(offsetof(MovementPostUpdateArbiter, deltaTime) == 0xBC);
	static_assert(offsetof(MovementPostUpdateArbiter, frameState) == 0xC0);
	static_assert(offsetof(MovementPostUpdateArbiter, storedLinear) == 0xC4);
	static_assert(offsetof(MovementPostUpdateArbiter, storedAngular) == 0xD0);
	static_assert(offsetof(MovementPostUpdateArbiter, previousStoredLinear) == 0xDC);
	static_assert(offsetof(MovementPostUpdateArbiter, hasStoredDeltas) == 0xE8);
	static_assert(offsetof(MovementPostUpdateArbiter, hadStoredDeltas) == 0xE9);
	static_assert(offsetof(MovementPostUpdateArbiter, interfaceRegistry) == 0xF0);
	static_assert(sizeof(MovementPostUpdateArbiter) == 0x100);
}
