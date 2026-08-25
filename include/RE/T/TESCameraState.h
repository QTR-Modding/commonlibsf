#pragma once

#include "RE/B/BSInputEventUser.h"
#include "RE/B/BSIntrusiveRefCounted.h"

namespace RE
{
	class PlayerCamera;

	class TESCameraState :
		public BSInputEventUser,      // 00
		public BSIntrusiveRefCounted  // 40
	{
	public:
		SF_RTTI_VTABLE(TESCameraState);

		~TESCameraState() override;  // 00

		// add
		virtual void Begin();  // 0A
		virtual void End();    // 0B

		// members
		std::uint32_t pad44;       // 44
		PlayerCamera* camera;      // 48
		std::uint32_t stateID;     // 50
		bool          unk54;       // 54
		std::byte     pad55[0x03];  // 55
	};
	static_assert(sizeof(TESCameraState) == 0x58);
	static_assert(offsetof(TESCameraState, camera) == 0x48);
}
