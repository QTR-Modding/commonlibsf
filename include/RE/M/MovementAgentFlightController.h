#pragma once

#include "RE/B/BSLock.h"
#include "RE/I/IMovementHandlerAgent.h"
#include "RE/I/IMovementPDControllerAccess.h"
#include "RE/I/IMovementPathManagerAgent.h"
#include "RE/I/IMovementPlannerAgent.h"
#include "RE/I/IMovementQueryPathFollowing.h"
#include "RE/I/IMovementQueryState.h"
#include "RE/I/IMovementSetRollTarget.h"
#include "RE/I/IMovementSetState.h"
#include "RE/M/MovementAgent.h"

namespace RE
{
	// Engine-owned flight agent. The interfaces operate on its physical movement
	// state; they do not directly set a reference pose or prove route clearance.
	class MovementAgentFlightController :
		public MovementAgent,                // 00
		public IMovementPlannerAgent,        // 18
		public IMovementHandlerAgent,        // 20
		public IMovementPathManagerAgent,    // 28
		public IMovementQueryPathFollowing,  // 30
		public IMovementQueryState,          // 38
		public IMovementSetState,            // 40
		public IMovementSetRollTarget,       // 48
		public IMovementPDControllerAccess   // 50
	{
	public:
		SF_RTTI_VTABLE(MovementAgentFlightController);

		~MovementAgentFlightController() override;

		// The engine retains the registry, request and path. Their internal types
		// and ownership APIs remain unmapped; callers must not replace these fields.
		void*             interfaceRegistry;  // 58
		std::uint32_t     unk60;              // 60
		std::uint32_t     pad64;              // 64
		void*             currentRequest;     // 68
		void*             currentPath;        // 70
		std::uint32_t     unk78;              // 78
		MovementStateData facingState;        // 7C
		float             unk98;              // 98
		float             unk9C;              // 9C
		NiPoint3          rollTarget;         // A0 - target up vector
		std::uint32_t     padAC;              // AC
		std::byte         unkB0[0x10];        // B0
		float             pdParameter1;       // C0
		float             pdParameter2;       // C4
		std::byte         unkC8[0x8];         // C8
		BSReadWriteLock   stateLock;          // D0 - guards facingState and rollTarget
		std::byte         unkD8[0x8];         // D8
	};
	static_assert(offsetof(MovementAgentFlightController, interfaceRegistry) == 0x58);
	static_assert(offsetof(MovementAgentFlightController, currentRequest) == 0x68);
	static_assert(offsetof(MovementAgentFlightController, currentPath) == 0x70);
	static_assert(offsetof(MovementAgentFlightController, facingState) == 0x7C);
	static_assert(offsetof(MovementAgentFlightController, rollTarget) == 0xA0);
	static_assert(offsetof(MovementAgentFlightController, pdParameter1) == 0xC0);
	static_assert(offsetof(MovementAgentFlightController, stateLock) == 0xD0);
	static_assert(sizeof(MovementAgentFlightController) == 0xE0);
}
