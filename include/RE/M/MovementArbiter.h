#pragma once

#include "RE/B/BSFixedString.h"
#include "RE/B/BSIntrusiveRefCounted.h"
#include "RE/B/BSTSmartPointer.h"

namespace RE
{
	class IMovementState;
	class MovementAgent;
	class BSPathingStreamRead;
	class BSPathingStreamWrite;

	// Opaque engine-owned inputs for the native pipeline.
	class MovementInterfaceRegistry;
	struct MovementUpdateData;
	struct MovementArbiterUpdateData;

	class MovementArbiter : public BSIntrusiveRefCounted
	{
	public:
		SF_RTTI_VTABLE(MovementArbiter);

		enum class Type : std::uint8_t
		{
			kHandler = 0,
			kPathManager = 1,
			kPlanner = 2,
			kPostUpdate = 3,
			kTweener = 4
		};

		virtual ~MovementArbiter();  // 00

		// Borrowed interned strings; the stage name selects the matching agent interface.
		virtual const BSFixedString& GetStageName() const = 0;  // 01
		virtual const BSFixedString& GetName() const = 0;       // 02
		virtual Type                 GetType() const = 0;       // 03
		// Registration and updates require the owning controller pipeline context.
		virtual void RegisterInterfaces(MovementInterfaceRegistry* a_registry);  // 04
		virtual void Unk_05();                                                   // 05
		virtual void Reset();                                                    // 06
		virtual void LoadConfiguration(BSPathingStreamRead& a_stream);           // 07
		virtual void BeginUpdate();                                              // 08
		virtual void Update(MovementUpdateData& a_data) = 0;                     // 09
		virtual void Unk_0A(MovementArbiterUpdateData& a_data) = 0;              // 0A
		virtual void Unk_0B();                                                   // 0B
		virtual void Unk_0C();                                                   // 0C
		virtual void UnregisterInterfaces();                                     // 0D
		// Native incremental I/O; stream failures are not reported by the arbiter.
		virtual void SaveState(BSPathingStreamWrite& a_stream);  // 0E
		virtual void LoadState(BSPathingStreamRead& a_stream);   // 0F
		// Borrows stage interfaces; the owning pipeline must keep attached agents alive.
		// Returns whether the agent exposes the stage interface, not allocation success.
		virtual bool AttachAgent(const BSTSmartPointer<MovementAgent>& a_agent);  // 10
		// Requires the matching stage interface to be attached already.
		virtual bool DetachAgent(const BSTSmartPointer<MovementAgent>& a_agent);  // 11
		virtual void ClearAgents();                                               // 12

		// Borrowed from the controller's active schema. Not reference-counted here.
		IMovementState* movementState;  // 10
	};
	static_assert(offsetof(MovementArbiter, movementState) == 0x10);
	static_assert(sizeof(MovementArbiter) == 0x18);
}
