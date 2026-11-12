#include "RE/S/SpaceshipTravel.h"
#include "RE/G/GameVM.h"
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

	bool CanLandAtMarker(TESObjectREFR* a_ship, TESObjectREFR* a_marker)
	{
		using func_t = decltype(&CanLandAtMarker);
		static REL::Relocation<func_t> func{ ID::Spaceship::Travel::CanLandAtMarker };
		return func(a_ship, a_marker);
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

	void CancelGravJumpInitiation(TESObjectREFR* a_ship, bool a_showNotification)
	{
		AbortGravJump(a_ship, a_showNotification);
		static REL::Relocation<bool*> pending{ ID::Spaceship::Travel::PendingGravInitiation };
		*pending = false;
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

RE::Spaceship::Travel::ShipLandingResult RE::Spaceship::Travel::RequestShipLanding(TESObjectREFR* a_ship, TESObjectREFR* a_marker)
{
	using parent_t = void (*)(ExtraDataList*, std::uint32_t*);
	using disable_t = void (*)(TESObjectREFR*);
	using enable_t = void (*)(TESObjectREFR*, bool);
	using link_t = void (*)(TESObjectREFR*, TESObjectREFR*, BGSKeyword*, bool);
	using request_t = bool (*)(TESObjectREFR*, std::uint8_t, bool, BSScript::IVirtualMachine**, std::uint32_t, bool*);
	static REL::Relocation<BSReadWriteLock*> actorServiceLock{ ID::Spaceship::Travel::ActorServiceLock };
	static REL::Relocation<BGSKeyword**> interactionKeyword{ ID::Spaceship::Travel::CurrentInteractionLinkKeyword };
	static REL::Relocation<parent_t> getParent{ ID::ExtraDataList::GetEnableStateParent };
	static REL::Relocation<disable_t> disable{ ID::TESObjectREFR::Disable };
	static REL::Relocation<enable_t> enable{ ID::TESObjectREFR::Enable };
	static REL::Relocation<link_t> setLink{ ID::TESObjectREFR::SetLinkedRef };
	static REL::Relocation<request_t> request{ ID::Spaceship::Travel::EnableWithLanding };
	const BSAutoWriteLock actorLock{ actorServiceLock.get() };
	constexpr auto temporary = static_cast<TESForm::FormFlags>(1U << 14);
	if (a_ship->IsDeleted() || a_ship->GetFormFlags().any(temporary)) {
		return ShipLandingResult::kRejected;
	}
	std::uint32_t parent{};
	getParent(a_ship->extraDataList.get(), std::addressof(parent));
	if (parent != 0) {
		return ShipLandingResult::kRejected;
	}
	const bool wasDisabled = a_ship->IsDisabled();
	if (wasDisabled) {
		const auto loaded = a_ship->loadedData.LockRead();
		if (*loaded && (*loaded)->data3D) {
			return ShipLandingResult::kRejected;
		}
	}
	GameVM* gameVM = GameVM::GetSingleton();
	BSScript::IVirtualMachine* vm = gameVM->GetVM();
	const BSAutoWriteLock queueLock{ gameVM->postRenderQueueLock };
	BSTFreeList<BSTSmartPointer<GameScript::DelayFunctor>>* pool{};
	if (gameVM->postRenderQueueToWriteTo == gameVM->postRenderFunctorQueue1) {
		pool = std::addressof(gameVM->postRenderFunctorPool1);
	} else if (gameVM->postRenderQueueToWriteTo == gameVM->postRenderFunctorQueue2) {
		pool = std::addressof(gameVM->postRenderFunctorPool2);
	}
	if (!vm || !pool || !pool->free) {
		return ShipLandingResult::kRejected;
	}
	const NiPointer<TESObjectREFR> previousLink{ a_ship->GetLinkedRef(*interactionKeyword) };
	disable(a_ship);
	if (!a_ship->IsDisabled()) {
		return ShipLandingResult::kRejected;
	}
	setLink(a_ship, a_marker, *interactionKeyword, true);
	bool accepted{};
	if (!request(a_ship, 3, false, std::addressof(vm), 0, std::addressof(accepted)) || !accepted) {
		setLink(a_ship, previousLink.get(), *interactionKeyword, true);
		if (!wasDisabled) {
			enable(a_ship, true);
		}
		return ShipLandingResult::kRejected;
	}
	return ShipLandingResult::kLandingQueued;
}

RE::TESObjectREFR* RE::Spaceship::Travel::GetLandingMarker(TESObjectREFR* a_ship)
{
	using func_t = decltype(&GetLandingMarker);
	static REL::Relocation<func_t> func{ ID::Spaceship::Travel::GetLandingMarker };
	return func(a_ship);
}
