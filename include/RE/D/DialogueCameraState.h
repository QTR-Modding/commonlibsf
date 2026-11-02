#pragma once

#include "RE/T/ThirdPersonState.h"

namespace RE
{
	class BGSCameraShot;
	class TESObjectREFR;

	class DialogueCameraState :
		public ThirdPersonState  // 000
	{
	public:
		SF_RTTI_VTABLE(DialogueCameraState);

		~DialogueCameraState() override;  // 00

		// Installs a native shot without entering the dialogue camera or changing input.
		// Call on the game thread with an initialized dialogue-camera owner and valid
		// subject/target references. Retain the shot for its use by the camera.
		void SetCameraShot(BGSCameraShot* a_shot, TESObjectREFR* a_subject,
			TESObjectREFR* a_target, bool a_notify = false)
		{
			using func_t = decltype(&DialogueCameraState::SetCameraShot);
			static REL::Relocation<func_t> func{ ID::DialogueCameraState::SetCameraShot };
			func(this, a_shot, a_subject, a_target, a_notify);
		}

		// Ends the explicit shot through native target cleanup and camera transition.
		// This does not leave the dialogue camera; the shot pointer can remain until
		// its next update. Call on the game thread for a shot owned by the caller.
		void EndCameraShot()
		{
			using func_t = decltype(&DialogueCameraState::EndCameraShot);
			static REL::Relocation<func_t> func{ ID::DialogueCameraState::EndCameraShot };
			func(this);
		}

		[[nodiscard]] BGSCameraShot* GetCameraShot() const noexcept { return cameraShot; }

		// members
		std::byte      unk2A8[0x2B0 - 0x2A8];  // 2A8
		BGSCameraShot* cameraShot;               // 2B0
		std::byte      unk2B8[0x388 - 0x2B8];  // 2B8
	};
	static_assert(sizeof(DialogueCameraState) == 0x388);
	static_assert(offsetof(DialogueCameraState, cameraShot) == 0x2B0);
}
