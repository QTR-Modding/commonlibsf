#pragma once

#include "RE/B/BSInputEventReceiver.h"
#include "RE/B/BSTEvent.h"
#include "RE/B/BSTSmartPointer.h"
#include "RE/D/DialogueCameraState.h"
#include "RE/T/TESCamera.h"
#include "RE/T/ThirdPersonState.h"

namespace RE
{
	class BSWorldOriginShiftEvent;
	class OtherEventEnabledEvent;
	class UserEventEnabledEvent;
	struct IdleInputEvent;

	namespace Spaceship
	{
		struct TakeOffEvent;
		struct DockEvent;
		struct LandedSetEvent;
	}

	struct CameraStates
	{
		enum CameraState : std::uint32_t
		{
			kFirstPerson = 0,
			kAutoVanity,
			kVATS,
			kIronSights,
			kPCTransition,
			kTween,
			kUnk06,
			kVehicle,
			kFlight,
			kShipFarTravel,
			kShipAction,
			kShipTargeting,
			kShipCombatOrbit,
			kFreeWalk,
			kFreeAdvanced,
			kFreeFly,
			kFreeTethered,
			kDialogue,
			kWorkshopIsometric,
			kPhotoMode,
			kThirdPerson,
			kFurniture,
			kHorse,
			kBleedout,

			kTotal,
		};
	};
	using CameraState = CameraStates::CameraState;

	class PlayerCamera :
		public TESCamera,                                // 00
		public BSInputEventReceiver,                     // 48
		public BSTEventSink<IdleInputEvent>,             // 58
		public BSTEventSink<UserEventEnabledEvent>,      // 60
		public BSTEventSink<OtherEventEnabledEvent>,     // 68
		public BSTEventSink<Spaceship::TakeOffEvent>,    // 70
		public BSTEventSink<Spaceship::DockEvent>,       // 78
		public BSTEventSink<Spaceship::LandedSetEvent>,  // 80
		public BSTEventSink<BSWorldOriginShiftEvent>,    // 88
		public BSTSingletonSDM<PlayerCamera>             // 90
	{
	public:
		SF_RTTI_VTABLE(PlayerCamera);

		~PlayerCamera() override;  // 00

		// override (TESCamera)
		void SetCameraRoot(NiNode* a_root) override;  // 01
		void SetEnabled(bool a_enabled) override;     // 02
		void Unk_04() override;                       // 04

		[[nodiscard]] static PlayerCamera* GetSingleton()
		{
			static REL::Relocation<PlayerCamera**> singleton{ ID::PlayerCamera::Singleton };
			return *singleton;
		}

		void ForceFirstPerson()
		{
			using func_t = decltype(&PlayerCamera::ForceFirstPerson);
			static REL::Relocation<func_t> func{ ID::PlayerCamera::ForceFirstPerson };
			return func(this);
		}

		void ForceThirdPerson()
		{
			using func_t = decltype(&PlayerCamera::ForceThirdPerson);
			static REL::Relocation<func_t> func{ ID::PlayerCamera::ForceThirdPerson };
			return func(this);
		}

		[[nodiscard]] DialogueCameraState* GetDialogueCameraState() noexcept
		{
			return static_cast<DialogueCameraState*>(cameraStates[CameraState::kDialogue]);
		}

		[[nodiscard]] const DialogueCameraState* GetDialogueCameraState() const noexcept
		{
			return static_cast<const DialogueCameraState*>(cameraStates[CameraState::kDialogue]);
		}

		[[nodiscard]] ThirdPersonState* GetThirdPersonState() noexcept
		{
			return static_cast<ThirdPersonState*>(cameraStates[CameraState::kThirdPerson]);
		}

		[[nodiscard]] const ThirdPersonState* GetThirdPersonState() const noexcept
		{
			return static_cast<const ThirdPersonState*>(cameraStates[CameraState::kThirdPerson]);
		}

		bool IsInFirstPerson() const
		{
			return QCameraEquals(CameraState::kFirstPerson);
		}

		bool IsInThirdPerson() const
		{
			return QCameraEquals(CameraState::kThirdPerson);
		}

		void SetCameraState(CameraState a_cameraState)
		{
			using func_t = decltype(&PlayerCamera::SetCameraState);
			static REL::Relocation<func_t> func{ ID::PlayerCamera::SetCameraState };
			return func(this, a_cameraState);
		}

		enum class FreeCameraExit
		{
			kInvalid,
			kRestored,
			kReleased
		};

		// Changes only the saved destination used when the current camera state restores its predecessor.
		[[nodiscard]] bool SetCameraRestoreState(CameraState a_cameraState);

		// Ends a non-freezing mode entered through ToggleFreeCameraMode. Pass the
		// predecessor captured before entry and the restore count immediately after it.
		// A takeover keeps its current camera; only this mode's saved frames are retired.
		// kRestored means the native predecessor was resumed, kReleased leaves the
		// current camera unchanged, and kInvalid means the restore array was not usable.
		[[nodiscard]] FreeCameraExit EndFreeCameraMode(CameraState a_cameraState,
			std::uint32_t a_restoreStateCount, CameraState a_previousState);

		[[nodiscard]] float GetWorldFOV() const noexcept { return worldFOV; }

		// Sets the current world FOV in degrees and refreshes the native frusta.
		// Call on the game thread; camera updates can replace this transient value.
		void SetWorldFOV(float a_degrees)
		{
			using func_t = decltype(&PlayerCamera::SetWorldFOV);
			static REL::Relocation<func_t> func{ ID::PlayerCamera::SetWorldFOV };
			func(this, a_degrees);
		}

		void ToggleFreeCameraMode(std::uint32_t a_cameraStateIndex, bool a_flag)
		{
			using func_t = decltype(&PlayerCamera::ToggleFreeCameraMode);
			static REL::Relocation<func_t> func{ ID::PlayerCamera::ToggleFreeCameraMode };
			return func(this, a_cameraStateIndex, a_flag);
		}

		bool QCameraEquals(CameraState a_cameraState) const
		{
			return (currentState == cameraStates[a_cameraState]);
		}

		// members
		std::byte     unkA0[0xB8 - 0xA0];               // 0A0
		std::uint32_t cameraRestoreStateCount;           // 0B8
		std::uint32_t padBC;                             // 0BC
		std::uint32_t cameraRestoreStateCapacityFlags;   // 0C0
		std::uint32_t padC4;                             // 0C4
		alignas(BSTSmartPointer<TESCameraState>)
			std::byte cameraRestoreStateStorage[0xC0];   // 0C8
		void* cameraStates[CameraState::kTotal];         // 188

		std::byte unk248[0x280 - 0x248];                 // 248
		float     worldFOV;                              // 280
		std::byte unk284[0x2E5 - 0x284];                 // 284
		bool      freeCameraMode;                        // 2E5

		// ...

	private:
		[[nodiscard]] std::optional<std::span<BSTSmartPointer<TESCameraState>>> GetCameraRestoreStates();
	};
	static_assert(offsetof(PlayerCamera, cameraRestoreStateCount) == 0xB8);
	static_assert(offsetof(PlayerCamera, cameraRestoreStateCapacityFlags) == 0xC0);
	static_assert(offsetof(PlayerCamera, cameraRestoreStateStorage) == 0xC8);
	static_assert(offsetof(PlayerCamera, cameraStates) == 0x188);
	static_assert(offsetof(PlayerCamera, worldFOV) == 0x280);
	static_assert(offsetof(PlayerCamera, freeCameraMode) == 0x2E5);
}
