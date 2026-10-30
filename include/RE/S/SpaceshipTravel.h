#pragma once

#include "RE/B/BSContainer.h"
#include "RE/B/BSFixedString.h"
#include "RE/B/BSPointerHandle.h"
#include "RE/B/BSTArray.h"
#include "RE/N/NiSmartPointer.h"

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

		enum class CelestialBodyType : std::uint8_t
		{
			kUndefined = 0,
			kStar = 1,
			kPlanet = 2,
			kMoon = 3,
			kSatellite = 4,
			kAsteroidBelt = 5,
			kStation = 6
		};

		struct CelestialBodyInfo
		{
			RoutePoint location;
			BSFixedString name;
			CelestialBodyType type;
		};

		struct SurfaceCoordinates
		{
			double longitude;  // radians
			double latitude;   // radians
		};
		static_assert(sizeof(SurfaceCoordinates) == 0x10);

		enum class LandingRequestMode : std::uint8_t
		{
			kHud = 0,
			kMap = 2
		};

		struct LandingSelection
		{
			SurfaceCoordinates coordinates{};
			BSPointerHandle<TESObjectREFR> marker;
			bool cleared{};
			bool validPoint{};
			LandingRequestMode mode{ LandingRequestMode::kHud };
		};

		struct LandingSiteInfo
		{
			BSPointerHandle<TESObjectREFR> marker;
			SurfaceCoordinates coordinates;
			BSFixedString nameText;
			BSFixedString extraText;
			BSFixedString questTargetText;
			bool visible;
		};

		enum class LandingRequestStatus : std::uint8_t
		{
			kRequested,
			kNotAtBody,
			kMarkerUnavailable,
			kDenied
		};

		struct LandingRequestResult
		{
			LandingRequestStatus status;
			BSFixedString denialReason;
		};

		// Current native celestial body ID, distinct from a system ID or TESFormID.
		// Call on the game thread after gameplay initialization.
		[[nodiscard]] std::uint32_t GetCurrentBodyID();

		// Resolves the nearest star, planet, moon or belt ancestor used by the ship
		// HUD's orbital-context comparison. Uses the containing ship when present.
		// Returns zero when no native body/ancestor is found, including unparented space.
		// Call on the game thread with a valid in-space reference.
		[[nodiscard]] std::uint32_t GetOrbitBodyID(TESObjectREFR* a_reference);

		// Native system containing this celestial body. Requires a valid body ID.
		[[nodiscard]] std::uint32_t GetBodySystemID(std::uint32_t a_bodyID);

		// Copies the native celestial kind; undefined when the body is absent.
		[[nodiscard]] CelestialBodyType GetBodyType(std::uint32_t a_bodyID);

		// Copies the native static-body list in engine order, restricted to the
		// reference body's system. Excludes the reference body and dynamic ships.
		// Names are owned; no database view or callback is retained. Call on the
		// game thread after gameplay initialization with a valid reference body.
		[[nodiscard]] BSTArray<CelestialBodyInfo> GetOtherBodiesInSystem(std::uint32_t a_referenceBodyID);

		// Visits references from the native space-body iterator in engine order.
		// Uses the source ship's FormID and includes references from space cells.
		// Callbacks run synchronously under native read views; do not change
		// body/cell membership. Copy a NiPointer to retain a reference.
		// Call on the game thread with a valid orbital ship. The caller supplies
		// range, visibility and gameplay filtering.
		BSContainer::ForEachResult ForEachSpaceReference(TESObjectREFR* a_source,
			std::function<BSContainer::ForEachResult(const NiPointer<TESObjectREFR>&)> a_callback);

		// Copies flat, ungrouped surface-marker rows on the current body, with stock
		// labels, coordinates and visibility. Excludes foot-to-ship actions and empty
		// markers. The caller owns presentation filtering and handle deduplication.
		// Returned text is owned; rows are not an admission result. Refresh actual
		// landing checks with RequestLandingAtMarker when the user selects a site.
		// Call on the game thread after gameplay initialization.
		[[nodiscard]] BSTArray<LandingSiteInfo> GetCurrentLandingSites();

		// Runs the full native takeoff request, including its authored action and
		// normal fallback. The return does not report acceptance or completion.
		// Call on the game thread with a valid undocked ship and caller admission.
		void RequestTakeOff(TESObjectREFR* a_ship);

		// Requests animated undocking for the ship's connected child/parent pairs.
		// True means at least one pair accepted the action, not completion. Listen
		// for DockEvent and check current topology to observe all pairs detached.
		// Call on the game thread with a valid ship and caller admission.
		[[nodiscard]] bool RequestUndock(TESObjectREFR* a_ship);

		// Runs the stock route executor. Depending on the route and ship state, this
		// can initiate a grav jump or same-system fast travel. It retains native
		// admission, power allocation and player-pilot classification.
		// Call on the game thread with a valid ship and the full accepted route.
		// The input is borrowed only for this call; the engine copies retained data.
		// A return does not report acceptance or asynchronous completion.
		void ExecuteRoute(TESObjectREFR* a_ship, const BSTArray<RoutePoint>& a_route);

		// Continue an animated request using the ship's stored route.
		// Returns false when no route is stored; jump events report initiation.
		bool ResumeGravJump(TESObjectREFR* a_ship);

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
		void RequestLanding(std::uint32_t a_bodyID, const SurfaceCoordinates& a_coordinates,
			BSPointerHandle<TESObjectREFR> a_marker, bool a_playAnimation, LandingRequestMode a_mode);

		// Recollects a surface marker on the current body and refreshes the stock
		// shared travel, location, ship-fit and selected-site landing checks before
		// requesting ordinary landing. Type-4 foot-to-ship sites are a separate action.
		// kRequested means the request was made, not asynchronous completion.
		// Call on the game thread after gameplay initialization.
		[[nodiscard]] LandingRequestResult RequestLandingAtMarker(std::uint32_t a_bodyID, BSPointerHandle<TESObjectREFR> a_marker);

		// Refreshes the shared travel, location and ship landing gates before
		// requesting a copied selection. Markers are recollected. Markerless points
		// retain the geometry flags from an accepted native map selection.
		// No map owner is retained. Call on the game thread after gameplay initialization.
		[[nodiscard]] LandingRequestResult RequestLandingAtSelection(std::uint32_t a_bodyID,
			const LandingSelection& a_selection);

	}
}
