#include "RE/S/SpaceshipTravel.h"

namespace RE::Spaceship::Travel
{
	void ExecuteRoute(TESObjectREFR* a_ship, const BSTArray<RoutePoint>& a_route)
	{
		using func_t = decltype(&ExecuteRoute);
		static REL::Relocation<func_t> func{ ID::Spaceship::Travel::ExecuteRoute };
		func(a_ship, a_route);
	}

	void AbortGravJump(TESObjectREFR* a_ship, bool a_showNotification)
	{
		using func_t = decltype(&AbortGravJump);
		static REL::Relocation<func_t> func{ ID::Spaceship::Travel::AbortGravJump };
		func(a_ship, a_showNotification);
	}

	bool ShouldPlayLandingAnimation(BSPointerHandle<TESObjectREFR> a_marker, std::uint32_t a_bodyID)
	{
		const auto marker = a_marker.get_handle();
		using func_t = bool (*)(const std::uint32_t*, std::uint32_t);
		static REL::Relocation<func_t> func{ ID::Spaceship::Travel::ShouldPlayLandingAnimation };
		return func(std::addressof(marker), a_bodyID);
	}

	void RequestLanding(std::uint32_t a_bodyID, const SurfaceCoordinates& a_coordinates, BSPointerHandle<TESObjectREFR> a_marker, bool a_playAnimation)
	{
		// Native result is a non-owning cell/pad/transform description. The engine
		// copies a 0x28-byte description; no result member or cleanup is needed here.
		alignas(std::uint64_t) std::byte result[0x28];
		using func_t = void* (*)(void*, std::uint32_t, const SurfaceCoordinates*, std::uint32_t, bool, std::int32_t);
		static REL::Relocation<func_t> func{ ID::Spaceship::Travel::RequestLanding };
		func(result, a_bodyID, std::addressof(a_coordinates), a_marker.get_handle(), a_playAnimation, 0);
	}
}
