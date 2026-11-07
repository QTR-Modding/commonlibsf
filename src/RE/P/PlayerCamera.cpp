#include "RE/P/PlayerCamera.h"

namespace RE
{
	std::optional<std::span<BSTSmartPointer<TESCameraState>>> PlayerCamera::GetCameraRestoreStates()
	{
		const auto capacity = cameraRestoreStateCapacityFlags & 0x7FFFFFFF;
		const bool local = (cameraRestoreStateCapacityFlags & 0x80000000) != 0;
		if (cameraRestoreStateCount > capacity ||
			(local && capacity > sizeof(cameraRestoreStateStorage) / sizeof(BSTSmartPointer<TESCameraState>))) {
			return std::nullopt;
		}
		if (cameraRestoreStateCount == 0) {
			return std::span<BSTSmartPointer<TESCameraState>>{};
		}
		BSTSmartPointer<TESCameraState>* states = local ? reinterpret_cast<BSTSmartPointer<TESCameraState>*>(cameraRestoreStateStorage) :
			*reinterpret_cast<BSTSmartPointer<TESCameraState>**>(cameraRestoreStateStorage);
		if (!states) {
			return std::nullopt;
		}
		return std::span{ states, cameraRestoreStateCount };
	}

	bool PlayerCamera::SetCameraRestoreState(CameraState a_cameraState)
	{
		if (a_cameraState >= CameraState::kTotal) {
			return false;
		}
		TESCameraState* state = static_cast<TESCameraState*>(cameraStates[a_cameraState]);
		if (!state) {
			return false;
		}
		const auto states = GetCameraRestoreStates();
		if (!states) {
			return false;
		}
		if (states->empty()) {
			return state == cameraStates[CameraState::kFirstPerson];
		}
		states->back().reset(state);
		return true;
	}

	PlayerCamera::FreeCameraExit PlayerCamera::EndFreeCameraMode(CameraState a_cameraState,
		std::uint32_t a_restoreStateCount, CameraState a_previousState)
	{
		if (a_cameraState < CameraState::kFreeWalk || a_cameraState > CameraState::kFreeTethered ||
			a_previousState >= CameraState::kTotal || a_restoreStateCount == 0) {
			return FreeCameraExit::kInvalid;
		}
		TESCameraState* owned = static_cast<TESCameraState*>(cameraStates[a_cameraState]);
		TESCameraState* previous = static_cast<TESCameraState*>(cameraStates[a_previousState]);
		const auto states = GetCameraRestoreStates();
		if (!states || !owned || !previous) {
			return FreeCameraExit::kInvalid;
		}

		const bool savedPrevious = a_restoreStateCount <= states->size() &&
			(*states)[a_restoreStateCount - 1].get() == previous;
		const bool savedOwned = a_restoreStateCount < states->size() &&
			(*states)[a_restoreStateCount].get() == owned;
		bool replacementFree = false;
		for (std::uint32_t index = CameraState::kFreeWalk; index <= CameraState::kFreeTethered; ++index) {
			if (currentState == cameraStates[index]) {
				replacementFree = currentState != owned || cameraRestoreStateCount != a_restoreStateCount;
				break;
			}
		}
		if (currentState == owned && !replacementFree && freeCameraMode) {
			ToggleFreeCameraMode(CameraState::kTotal, false);
			return FreeCameraExit::kRestored;
		}
		if (!savedOwned && (!savedPrevious || replacementFree || !freeCameraMode)) {
			return FreeCameraExit::kReleased;
		}

		// Pushed takeovers keep their predecessor; direct forcing leaves only our entry frame.
		const auto removed = savedOwned ? a_restoreStateCount : a_restoreStateCount - 1;
		for (std::uint32_t index = removed; index + 1 < cameraRestoreStateCount; ++index) {
			(*states)[index] = std::move((*states)[index + 1]);
		}
		states->back().reset();
		--cameraRestoreStateCount;
		if (!replacementFree) {
			freeCameraMode = false;
		}
		return FreeCameraExit::kReleased;
	}
}
