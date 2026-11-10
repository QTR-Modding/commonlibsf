#include "RE/T/TESObjectREFR.h"

#include "RE/A/Actor.h"
#include "RE/B/BGSInventoryItem.h"
#include "RE/B/BGSKeyword.h"
#include "RE/C/CrewExtraData.h"
#include "RE/E/ExtraLock.h"

namespace RE
{
	namespace
	{
		// Reference branch of GameScript::RefrOrInventoryObj. The inventory
		// branch remains empty; the native wrapper resolves the reference's source.
		struct ReferenceSource
		{
			NiPointer<TESObjectREFR> reference;
			std::byte                inventory[0x10]{};
		};
		static_assert(offsetof(ReferenceSource, reference) == 0);
		static_assert(sizeof(ReferenceSource) == 0x18);

		using LinkedParentCallback = std::function<void(BGSKeyword*, TESObjectREFR*)>;

		struct LinkedParentVisitor
		{
			struct VTable
			{
				std::uintptr_t opaque00;
				std::uint64_t (*invoke)(LinkedParentVisitor*, BGSKeyword*, TESObjectREFR*);
			};

			static std::uint64_t Invoke(LinkedParentVisitor* a_self, BGSKeyword* a_keyword, TESObjectREFR* a_parent)
			{
				if (a_parent) {
					(*a_self->callback)(a_keyword, a_parent);
				}
				return 1;
			}

			const VTable*               vtable;
			const LinkedParentCallback* callback;
		};
		static_assert(std::is_standard_layout_v<LinkedParentVisitor>);
		static_assert(offsetof(LinkedParentVisitor::VTable, invoke) == 0x8);
		constexpr LinkedParentVisitor::VTable linkedParentVTable{ 0, LinkedParentVisitor::Invoke };
	}

	void TESObjectREFR::ForEachEquippedItem(std::function<BSContainer::ForEachResult(const BGSInventoryItem&)> a_callback) const
	{
		ForEachInventoryItem([&](const BGSInventoryItem& a_invItem) {
			return a_invItem.IsEquipped() ? a_callback(a_invItem) : BSContainer::ForEachResult::kContinue;
		});
	}

	void TESObjectREFR::ForEachInventoryItem(std::function<BSContainer::ForEachResult(const BGSInventoryItem&)> a_callback) const
	{
		if (const auto invList = inventoryList.LockRead(); *invList) {
			for (const auto& invItem : invList->data) {
				if (invItem.object && a_callback(invItem) == BSContainer::ForEachResult::kStop) {
					break;
				}
			}
		}
	}

	Actor* TESObjectREFR::GetActorRefOwner() const
	{
		using func_t = Actor* (*)(ExtraDataList*);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::GetActorRefOwner };
		return extraDataList ? func(extraDataList.get()) : nullptr;
	}

	TESFaction* TESObjectREFR::GetFactionOwner() const
	{
		using func_t = TESFaction* (*)(ExtraDataList*);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::GetFactionOwner };
		return extraDataList ? func(extraDataList.get()) : nullptr;
	}

	BGSLocation* TESObjectREFR::GetCurrentLocation()
	{
		using func_t = decltype(&TESObjectREFR::GetCurrentLocation);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::GetCurrentLocation };
		return func(this);
	}

	std::vector<NiPointer<TESObjectREFR>> TESObjectREFR::GetCrewMembers()
	{
		using func_t = CrewExtraData* (*)(TESObjectREFR*, bool);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::GetCrewData };
		std::vector<NiPointer<TESObjectREFR>> result;
		if (auto* crew = func(this, false)) {
			result.reserve(crew->members.size());
			for (auto& handle : crew->members) {
				if (auto member = handle.get()) {
					result.push_back(std::move(member));
				} else {
					handle.reset();
				}
			}
		}
		return result;
	}

	std::vector<NiPointer<TESObjectREFR>> TESObjectREFR::GetExteriorRefs(BGSKeyword* a_keyword)
	{
		BSScrapArray<TESObjectREFR*> references;
		using func_t = BSScrapArray<TESObjectREFR*>* (*)(BSScrapArray<TESObjectREFR*>*, BGSKeyword*, TESObjectREFR*);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::GetExteriorRefs };
		func(std::addressof(references), a_keyword, this);
		std::vector<NiPointer<TESObjectREFR>> result;
		result.reserve(references.size());
		for (auto* reference : references) {
			result.emplace_back(reference);
		}
		return result;
	}

	std::vector<NiPointer<TESObjectREFR>> TESObjectREFR::GetLandingRamps()
	{
		static REL::Relocation<BGSKeyword**> keyword{ ID::TESObjectREFR::SpaceshipRampKeyword };
		BGSKeyword*                          rampKeyword = *keyword;
		if (!rampKeyword || !rampKeyword->Is(FormType::kKYWD)) {
			return {};
		}
		return GetExteriorRefs(rampKeyword);
	}

	float TESObjectREFR::GetDistance(NiPointer<TESObjectREFR> a_other, bool a_disabledRefs, bool a_ignoreWorldspace) const
	{
		using func_t = decltype(&TESObjectREFR::GetDistance);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::GetDistance };
		return func(this, a_other, a_disabledRefs, a_ignoreWorldspace);
	}

	TESObjectREFR* TESObjectREFR::GetLinkedRef(BGSKeyword* a_keyword)
	{
		using func_t = TESObjectREFR* (*)(ExtraDataList*, BGSKeyword*, bool);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::GetLinkedRef };
		return extraDataList ? func(extraDataList.get(), a_keyword, true) : nullptr;
	}

	void TESObjectREFR::ForEachLinkedParent(LinkedParentCallback a_callback) const
	{
		if (extraDataList) {
			LinkedParentVisitor visitor{ std::addressof(linkedParentVTable), std::addressof(a_callback) };
			using func_t = std::uint64_t (*)(ExtraDataList*, LinkedParentVisitor*);
			static REL::Relocation<func_t> func{ ID::ExtraDataList::ForEachLinkedParent };
			func(extraDataList.get(), std::addressof(visitor));
		}
	}

	REFR_LOCK* TESObjectREFR::GetLock() const
	{
		using func_t = decltype(&TESObjectREFR::GetLock);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::GetLock };
		return func(this);
	}

	LOCK_LEVEL TESObjectREFR::GetLockLevel() const
	{
		const auto state = GetLock();
		return state ? state->GetLockLevel(this) : LOCK_LEVEL::kUnlocked;
	}

	TESWorldSpace* TESObjectREFR::GetParentWorldSpace()
	{
		using func_t = decltype(&TESObjectREFR::GetParentWorldSpace);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::GetParentWorldSpace };
		return func(this);
	}

	bool TESObjectREFR::GetSpaceTransform(NiPoint3A& a_position, NiMatrix3& a_rotation) const
	{
		using func_t = bool (*)(const TESObjectREFR*, NiPoint3A*, NiMatrix3*);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::GetSpaceTransform };
		return func(this, std::addressof(a_position), std::addressof(a_rotation));
	}

	float TESObjectREFR::GetScale() const
	{
		using func_t = decltype(&TESObjectREFR::GetScale);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::GetScale };
		return func(this);
	}

	TESObjectREFR* TESObjectREFR::GetSpaceship(bool a_arg1)
	{
		using func_t = decltype(&TESObjectREFR::GetSpaceship);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::GetSpaceship };
		return func(this, a_arg1);
	}

	TESObjectREFR* TESObjectREFR::GetSpaceshipParentDock()
	{
		using func_t = decltype(&TESObjectREFR::GetSpaceshipParentDock);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::GetSpaceshipParentDock };
		return func(this);
	}

	Actor* TESObjectREFR::GetSpaceshipPilot()
	{
		using func_t = decltype(&TESObjectREFR::GetSpaceshipPilot);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::GetSpaceshipPilot };
		return func(this);
	}

	TESObjectREFR* TESObjectREFR::GetSpaceshipPilotSeat()
	{
		using func_t = decltype(&TESObjectREFR::GetSpaceshipPilotSeat);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::GetSpaceshipPilotSeat };
		return func(this);
	}

	NiPointer<Actor> TESObjectREFR::GetSpaceshipPilotSeatOccupant()
	{
		BSPointerHandle<Actor> occupant;
		using func_t = BSPointerHandle<Actor>* (*)(BSPointerHandle<Actor>*, TESObjectREFR*);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::GetSpaceshipPilotSeatOccupant };
		func(std::addressof(occupant), this);
		return occupant.get();
	}

	std::int32_t TESObjectREFR::GetValue()
	{
		using func_t = decltype(&TESObjectREFR::GetValue);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::GetValue };
		return func(this);
	}

	bool TESObjectREFR::HasKeyword(BGSKeyword* a_keyword)
	{
		using func_t = decltype(&TESObjectREFR::HasKeyword);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::HasKeyword };
		return func(this, a_keyword);
	}

	bool TESObjectREFR::IsCrimeToActivate()
	{
		using func_t = decltype(&TESObjectREFR::IsCrimeToActivate);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::IsCrimeToActivate };
		return func(this);
	}

	std::uint32_t TESObjectREFR::GetAvailableMarker(TESObjectREFR* a_user, bool a_ignoreReservations)
	{
		using func_t = decltype(&TESObjectREFR::GetAvailableMarker);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::GetAvailableFurnitureMarker };
		return func(this, a_user, a_ignoreReservations);
	}

	bool TESObjectREFR::SetMarkerReserved(std::uint32_t a_marker, TESObjectREFR* a_user, bool a_reserved, bool a_force)
	{
		using func_t = decltype(&TESObjectREFR::SetMarkerReserved);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::SetMarkerReserved };
		return func(this, a_marker, a_user, a_reserved, a_force);
	}

	std::uint32_t TESObjectREFR::GetAvailableFurnitureMarker(Actor* a_actor, bool a_ignoreReservations)
	{
		return IsFurniture() ? GetAvailableMarker(a_actor, a_ignoreReservations) : static_cast<std::uint32_t>(-1);
	}

	bool TESObjectREFR::IsFurnitureMarkerInUse(std::uint32_t a_marker, bool a_ignoreReservations)
	{
		using func_t = decltype(&TESObjectREFR::IsFurnitureMarkerInUse);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::IsFurnitureMarkerInUse };
		return IsFurniture() && func(this, a_marker, a_ignoreReservations);
	}

	bool TESObjectREFR::IsInSpace(bool a_arg1)
	{
		using func_t = decltype(&TESObjectREFR::IsInSpace);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::IsInSpace };
		return func(this, a_arg1);
	}

	bool TESObjectREFR::IsInSameSpace(const TESObjectREFR* a_other, bool a_allowParentWorld) const
	{
		using func_t = decltype(&TESObjectREFR::IsInSameSpace);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::IsInSameSpace };
		return func(this, a_other, a_allowParentWorld);
	}

	bool TESObjectREFR::IsLocked() const
	{
		return GetLockLevel() != LOCK_LEVEL::kUnlocked;
	}

	bool TESObjectREFR::IsObjectEquipped(TESBoundObject* a_object)
	{
		using func_t = decltype(&TESObjectREFR::IsObjectEquipped);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::IsObjectEquipped };
		return func(this, a_object);
	}

	bool TESObjectREFR::IsSpaceshipDocked()
	{
		using func_t = decltype(&TESObjectREFR::IsSpaceshipDocked);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::IsSpaceshipDocked };
		return func(this);
	}

	bool TESObjectREFR::IsSpaceshipLanded()
	{
		using func_t = decltype(&TESObjectREFR::IsSpaceshipLanded);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::IsSpaceshipLanded };
		return func(this);
	}

	bool TESObjectREFR::WornHasKeyword(BGSKeyword* a_keyword)
	{
		using func_t = decltype(&TESObjectREFR::WornHasKeyword);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::WornHasKeyword };
		return func(this, a_keyword);
	}

	bool TESObjectREFR::PlayAnimation(const BSFixedString& a_eventName)
	{
		return !IsActor() && NotifyAnimationGraphImpl(a_eventName, false);
	}

	void TESObjectREFR::DamageValue(const ActorValueInfo& a_value, float a_amount)
	{
		using func_t = void (*)(void*, std::uint32_t, TESObjectREFR*, const ActorValueInfo*, float);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::DamageValue };
		func(nullptr, 0, this, std::addressof(a_value), a_amount);
	}

	bool TESObjectREFR::PushActorAway(Actor& a_actor, float a_force)
	{
		const auto* process = a_actor.currentProcess;
		if (!process || process->unkE7 != 0 || !process->middleHigh) {
			return false;
		}
		const ReferenceSource source{ NiPointer<TESObjectREFR>{ this } };
		using func_t = void (*)(void*, std::uint32_t, const ReferenceSource*, Actor*, float);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::PushActorAway };
		func(nullptr, 0, std::addressof(source), std::addressof(a_actor), a_force);
		return true;
	}

	TESObjectREFR::OpenState TESObjectREFR::GetOpenState() const
	{
		using func_t = decltype(&TESObjectREFR::GetOpenState);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::GetOpenState };
		return func(this);
	}

	void TESObjectREFR::SetOpen(bool a_open)
	{
		using func_t = void (*)(void*, std::uint32_t, TESObjectREFR*, bool);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::SetOpen };
		func(nullptr, 0, this, a_open);
	}

	void TESObjectREFR::Lock()
	{
		if (const auto lock = GetLock()) {
			lock->SetLocked(true);
			AddLockChange();
		}
	}

	void TESObjectREFR::Unlock()
	{
		if (const auto lock = GetLock()) {
			lock->SetLocked(false);
			AddLockChange();
		}
	}

	void TESObjectREFR::AddLockChange()
	{
		using func_t = decltype(&TESObjectREFR::AddLockChange);
		static REL::Relocation<func_t> func{ ID::TESObjectREFR::AddLockChange };
		return func(this);
	}
}

RE::TESBoundObject* RE::TESObjectREFR::GetBaseObjectForComponent(const BSFixedString& a_componentName)
{
	using func_t = TESBoundObject* (*)(TESObjectREFR*, const BSFixedString&);
	static REL::Relocation<func_t> func{ ID::TESObjectREFR::GetBaseObjectForComponent };
	return func(this, a_componentName);
}

std::uint32_t RE::TESObjectREFR::GetSpaceBodyID() const
{
	std::uint32_t body{};
	using func_t = std::uint32_t* (*)(std::uint32_t*, const TESObjectREFR*);
	static REL::Relocation<func_t> resolve{ ID::Spaceship::Travel::ResolveReferenceBody };
	resolve(std::addressof(body), this);
	return body;
}

bool RE::TESObjectREFR::HasDynamicState() const
{
	using func_t = bool (*)(TESFormID);
	static REL::Relocation<func_t> query{ ID::TESObjectREFR::HasDynamicState };
	const std::uint32_t body = GetSpaceBodyID();
	return body && query(body);
}
