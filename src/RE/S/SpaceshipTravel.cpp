#include "RE/S/SpaceshipTravel.h"
#include "RE/T/TESObjectREFR.h"
#include "SpaceshipComponentLease.h"

namespace RE::Spaceship::Travel
{
	std::uint32_t GetCurrentBodyID()
	{
		std::uint32_t bodyID;
		using func_t = std::uint32_t* (*)(std::uint32_t*);
		static REL::Relocation<func_t> func{ ID::Spaceship::Travel::GetCurrentBodyID };
		func(std::addressof(bodyID));
		return bodyID;
	}

	void RequestTakeOff(TESObjectREFR* a_ship)
	{
		using func_t = decltype(&RequestTakeOff);
		static REL::Relocation<func_t> func{ ID::Spaceship::Travel::RequestTakeOff };
		func(a_ship);
	}

	bool RequestUndock(TESObjectREFR* a_ship)
	{
		using func_t = decltype(&RequestUndock);
		static REL::Relocation<func_t> func{ ID::Spaceship::Travel::RequestUndock };
		return func(a_ship);
	}

	void ExecuteRoute(TESObjectREFR* a_ship, const BSTArray<RoutePoint>& a_route)
	{
		using func_t = decltype(&ExecuteRoute);
		static REL::Relocation<func_t> func{ ID::Spaceship::Travel::ExecuteRoute };
		func(a_ship, a_route);
	}

	bool ResumeGravJump(TESObjectREFR* a_ship)
	{
		using manager_t = std::byte* (*)();
		static REL::Relocation<manager_t> manager{ ID::BSGalaxy::GetKnowledgeManager };
		using RouteLease = detail::ComponentLease<void>;
		using getter_t = RouteLease* (*)(void*, RouteLease*, void*, const TESFormID*);
		static REL::Relocation<getter_t> getter{ ID::Spaceship::Travel::GetStoredRoute };
		using resume_t = void (*)(TESObjectREFR*, void*);
		static REL::Relocation<resume_t> resume{ ID::Spaceship::Travel::ResumeGravJump };
		RouteLease route;
		const auto identity = a_ship->GetFormID();
		getter(*reinterpret_cast<void**>(manager() + 0x8B0), &route, nullptr, &identity);
		if (!route.value) {
			return false;
		}
		resume(a_ship, route.value);
		return true;
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
		RequestLanding(a_bodyID, a_coordinates, a_marker, a_playAnimation, LandingRequestMode::kHud);
	}

	void RequestLanding(std::uint32_t a_bodyID, const SurfaceCoordinates& a_coordinates,
		BSPointerHandle<TESObjectREFR> a_marker, bool a_playAnimation, LandingRequestMode a_mode)
	{
		// Native result is a non-owning cell/pad/transform description. The engine
		// copies a 0x28-byte description; no result member or cleanup is needed here.
		alignas(std::uint64_t) std::byte result[0x28];
		using func_t = void* (*)(void*, std::uint32_t, const SurfaceCoordinates*, std::uint32_t, bool, std::int32_t);
		static REL::Relocation<func_t> func{ ID::Spaceship::Travel::RequestLanding };
		func(result, a_bodyID, std::addressof(a_coordinates), a_marker.get_handle(), a_playAnimation, static_cast<std::int32_t>(a_mode));
	}
}
