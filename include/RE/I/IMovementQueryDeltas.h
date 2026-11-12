#pragma once

#include "RE/I/IMovementInterface.h"
#include "RE/N/NiPoint.h"

namespace RE
{
	class IMovementQueryDeltas : public IMovementInterface
	{
	public:
		SF_RTTI(IMovementQueryDeltas);

		// Consumer-owned output storage for GetUpdateData, not the complete native frame type.
		// The native getter writes through offset 0x43 and preserves unk38.
		struct UpdateData
		{
			NiPoint3      rotationOffset;   // 00
			std::uint32_t unk0C{};          // 0C
			NiPoint3      angularVelocity;  // 10
			std::uint32_t unk1C{};          // 1C
			std::byte     unk20[0x10]{};    // 20
			float         speed{};          // 30
			std::uint32_t unk34{};          // 34
			std::byte     unk38[0x8]{};     // 38
			float         deltaTime{};      // 40
		};
		static_assert(sizeof(UpdateData) == 0x44);
		static_assert(alignof(UpdateData) == 0x4);
		static_assert(offsetof(UpdateData, rotationOffset) == 0x00);
		static_assert(offsetof(UpdateData, angularVelocity) == 0x10);
		static_assert(offsetof(UpdateData, speed) == 0x30);
		static_assert(offsetof(UpdateData, deltaTime) == 0x40);

		struct CorrectionVelocities
		{
			NiPoint3 linear;   // 00
			NiPoint3 angular;  // 0C
		};
		static_assert(sizeof(CorrectionVelocities) == 0x18);
		static_assert(offsetof(CorrectionVelocities, angular) == 0xC);

		~IMovementQueryDeltas() override;  // 00

		// The engine owns this interface and its movement-update context. These calls do not lock.
		virtual float GetSpeed() const = 0;  // 01

		// False preserves the output. Requires current stored deltas and a positive frame delta.
		virtual bool     GetIntegratedSpeed(float& a_speed) const = 0;  // 02
		virtual NiPoint3 GetRotationOffset() const = 0;                 // 03

		// Derives Euler angles from the stored linear increment, with the native zero-motion fallback.
		// False preserves the output.
		virtual bool GetIntegratedDirectionAngles(NiPoint3& a_angles) const = 0;  // 04

		// Borrowed pipeline storage; copy the value before retaining it across updates.
		virtual const NiPoint3& GetAngularVelocity() const = 0;  // 05

		// False preserves the output. The native implementation does not check for a zero frame delta.
		virtual bool         GetIntegratedAngularVelocity(NiPoint3& a_angularVelocity) const = 0;  // 06
		virtual float        GetDeltaTime() const = 0;                                             // 07
		virtual std::uint8_t Unk_08() const = 0;                                                   // 08

		// Adds the wrapped change in rotation offset per second to the requested angular velocity.
		// The native movement-state/override checks can instead return a zero vector.
		virtual NiPoint3 GetAdjustedAngularVelocity() const = 0;  // 09
		virtual bool     Unk_0A(NiPoint3& a_result) const = 0;    // 0A

		virtual void GetStartPose(NiPoint3& a_position, NiPoint3& a_angles) const = 0;      // 0B
		virtual void GetPredictedPose(NiPoint3& a_position, NiPoint3& a_angles) const = 0;  // 0C

		// False preserves both outputs; otherwise integrates the stored local increments from the start pose.
		virtual bool GetIntegratedPose(NiPoint3& a_position, NiPoint3& a_angles) const = 0;  // 0D

		// Each output is independently optional. Initialize UpdateData before calling because unk38 is untouched.
		virtual void GetUpdateData(UpdateData* a_data, CorrectionVelocities* a_corrections) const = 0;  // 0E

		// Copies integrated linear/angular increments when available in the current frame.
		// False leaves both outputs unchanged. This is not a pending-request completion test.
		virtual bool GetStoredDeltas(NiPoint3& a_linear, NiPoint3& a_angular) const = 0;  // 0F
	};
	static_assert(sizeof(IMovementQueryDeltas) == 0x8);
}
