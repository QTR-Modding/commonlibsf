#pragma once

#include "RE/T/TESCameraState.h"

namespace RE
{
	class ThirdPersonState :
		public TESCameraState  // 000
	{
	public:
		SF_RTTI_VTABLE(ThirdPersonState);

		~ThirdPersonState() override;  // 00

		[[nodiscard]] float GetCameraTargetZoom() const noexcept
		{
			return cameraTargetZoom;
		}

		[[nodiscard]] float GetCameraCurrentZoom() const noexcept
		{
			return cameraCurrentZoom;
		}

		[[nodiscard]] bool IsCameraNearFarMode() const noexcept
		{
			return cameraNearFarMode;
		}

		// Selects the native second (far) third-person camera stage.
		void EnableCameraNearFarMode()
		{
			using func_t = decltype(&ThirdPersonState::EnableCameraNearFarMode);
			static REL::Relocation<func_t> func{ ID::ThirdPersonState::EnableCameraNearFarMode };
			return func(this);
		}

		// members
		std::byte unk058[0x224 - 0x058];  // 058
		float     cameraTargetZoom;        // 224
		float     cameraCurrentZoom;       // 228
		std::byte unk22C[0x28A - 0x22C];  // 22C
		bool      cameraNearFarMode;       // 28A
		std::byte unk28B[0x2A8 - 0x28B];  // 28B
	};
	static_assert(sizeof(ThirdPersonState) == 0x2A8);
	static_assert(offsetof(ThirdPersonState, cameraTargetZoom) == 0x224);
	static_assert(offsetof(ThirdPersonState, cameraCurrentZoom) == 0x228);
	static_assert(offsetof(ThirdPersonState, cameraNearFarMode) == 0x28A);
}
