#pragma once

#include "RE/B/BSPointerHandle.h"
#include "RE/B/BSTArray.h"

namespace RE
{
	class TESObjectREFR;

	namespace Spaceship::Travel
	{
		// Native celestial identities, in the exact order accepted by the star map.
		// A body ID is not necessarily a TESFormID or a reference handle.
		struct RoutePoint
		{
			std::uint32_t systemID;
			std::uint32_t bodyID;
		};
		static_assert(sizeof(RoutePoint) == 0x8);
		static_assert(sizeof(BSTArray<RoutePoint>) == 0x10);

		struct SurfaceCoordinates
		{
			double longitude;  // radians
			double latitude;   // radians
		};
		static_assert(sizeof(SurfaceCoordinates) == 0x10);

		// Runs the stock route executor. Depending on the route and ship state, this
		// can initiate a grav jump or same-system fast travel. It retains native
		// admission, power allocation and player-pilot classification.
		// Call on the game thread with a valid ship and the full accepted route.
		// The input is borrowed only for this call; the engine copies retained data.
		// A return does not report acceptance or asynchronous completion.
		void ExecuteRoute(TESObjectREFR* a_ship, const BSTArray<RoutePoint>& a_route);

		// Aborts an active native grav jump through its normal action, AV cleanup
		// and state-3 event. It does not clear the global accepted route or cancel
		// a separately owned AI path. The notification retains native player gating.
		// Call on the game thread with the valid ship whose active jump is owned.
		void AbortGravJump(TESObjectREFR* a_ship, bool a_showNotification);

		// Stock HUD choice for the landing-animation flag. The handle identifies
		// the selected surface marker; bodyID is its native celestial body ID.
		[[nodiscard]] bool ShouldPlayLandingAnimation(BSPointerHandle<TESObjectREFR> a_marker, std::uint32_t a_bodyID);

		// Requests ordinary native landing for the player's containing/home ship,
		// using stock cell/pad selection and player travel-request bookkeeping.
		// Requires fresh, matching body/site inputs and successful caller admission
		// (travel permission, route, combat, jump and selected-site landing gates).
		// Call on the game thread after gameplay initialization. Coordinates and
		// handle are copied synchronously; no HUD or map owner must remain alive.
		// The return does not report acceptance or asynchronous completion.
		void RequestLanding(std::uint32_t a_bodyID, const SurfaceCoordinates& a_coordinates, BSPointerHandle<TESObjectREFR> a_marker, bool a_playAnimation);
	}
}
