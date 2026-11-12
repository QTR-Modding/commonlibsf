#pragma once

#include "RE/N/NiPoint.h"

namespace RE
{
	class TESObjectREFR;

	// Retains a native component view. Call on the game thread after gameplay initialization.
	class ViewSpace
	{
	public:
		struct Origin
		{
			std::array<double, 4> floatingOrigin;
			std::array<double, 4> anchor;
			std::array<double, 4> fittedPosition;
			NiPoint3 velocity;
			bool hasHighDetailInfo{};
		};

		ViewSpace() noexcept;
		~ViewSpace();
		ViewSpace(ViewSpace&& a_rhs) noexcept;
		ViewSpace& operator=(ViewSpace&& a_rhs) noexcept;
		ViewSpace(const ViewSpace&) = delete;
		ViewSpace& operator=(const ViewSpace&) = delete;
		[[nodiscard]] explicit operator bool() const noexcept;

		[[nodiscard]] static ViewSpace Capture();
		// Selects a system component view and synchronizes PlayerCamera ownership.
		// Coordinate cell and world loading separately.
		static void SelectSystem(std::uint32_t a_systemID);
		// Selects the reference's native space body and fits the current camera to it.
		// Apply caller-owned camera presentation afterward; this does not move the player.
		static void SelectReference(std::uint32_t a_systemID, TESObjectREFR* a_reference);
		static void SetFloatingOrigin(const std::array<double, 4>& a_origin);
		// Copies or restores this view's anchor, fit and velocity plus the shared floating origin.
		// A view without high-detail data captures only the floating origin.
		// Failure leaves the output or native state unchanged.
		[[nodiscard]] bool CaptureOrigin(Origin& a_out) const;
		[[nodiscard]] bool RestoreOrigin(const Origin& a_origin) const;
		// Requires a valid captured view; this handle keeps its ownership afterward.
		void Select() const;

	private:
		void Release() noexcept;

		void* _space{};
	};
}
