#include "RE/S/SpaceshipTravel.h"

#include "RE/T/TESObjectREFR.h"

namespace RE::Spaceship::Travel
{
	namespace
	{
		using ReferenceCallback = std::function<BSContainer::ForEachResult(const NiPointer<TESObjectREFR>&)>;
		using BodyCallback = BSContainer::ForEachResult (*)(void*, std::uint32_t, const void*, const void*, const NiPointer<TESObjectREFR>&);

		// The synchronous iterator reads only target.vtable[2], never the two
		// preceding slots. It neither copies, destroys nor retains this target.
		// Borrowed BodyDrawData arguments remain opaque to these collectors.
		struct BodyCollectorDispatch
		{
			std::uintptr_t unusedSlots[2]{};
			BodyCallback invoke;
		};
		static_assert(offsetof(BodyCollectorDispatch, invoke) == 0x10);

		struct BodyCollector
		{
			const BodyCollectorDispatch* dispatch;
			std::uint32_t systemID;
			BSTArray<CelestialBodyInfo>* bodies;
		};
		static_assert(offsetof(BodyCollector, dispatch) == 0x00);

		struct ReferenceCollector
		{
			const BodyCollectorDispatch* dispatch;
			const ReferenceCallback* callback;
		};
		static_assert(offsetof(ReferenceCollector, dispatch) == 0x00);

		BSContainer::ForEachResult CollectBody(void* a_context, std::uint32_t a_bodyID, const void*, const void*, const NiPointer<TESObjectREFR>&)
		{
			const auto* collector = static_cast<BodyCollector*>(a_context);
			const auto systemID = GetBodySystemID(a_bodyID);
			if (systemID == collector->systemID) {
				BSFixedString name;
				using func_t = BSFixedString* (*)(BSFixedString*, std::uint32_t);
				static REL::Relocation<func_t> func{ ID::Spaceship::Travel::GetCelestialBodyName };
				func(std::addressof(name), a_bodyID);
				collector->bodies->push_back({ { systemID, a_bodyID }, std::move(name), GetBodyType(a_bodyID) });
			}
			return BSContainer::ForEachResult::kContinue;
		}

		BSContainer::ForEachResult CollectReference(void* a_context, std::uint32_t a_bodyID, const void*, const void*, const NiPointer<TESObjectREFR>& a_reference)
		{
			const auto* collector = static_cast<ReferenceCollector*>(a_context);
			if (a_reference) {
				return (*collector->callback)(a_reference);
			}
			auto* form = TESForm::LookupByID(a_bodyID);
			NiPointer<TESObjectREFR> reference{ form ? form->AsReference() : nullptr };
			return reference ? (*collector->callback)(reference) : BSContainer::ForEachResult::kContinue;
		}

		BSContainer::ForEachResult VisitBodies(std::uint32_t a_sourceID, bool a_includeCellReferences, void* a_visitor)
		{
			using func_t = decltype(&VisitBodies);
			static REL::Relocation<func_t> func{ ID::Spaceship::Travel::ForEachCelestialBody };
			return func(a_sourceID, a_includeCellReferences, a_visitor);
		}
	}

	std::uint32_t GetOrbitBodyID(TESObjectREFR* a_reference)
	{
		if (auto* ship = a_reference->GetSpaceship(true)) {
			a_reference = ship;
		}
		std::uint32_t bodyID{};
		using resolve_body_t = std::uint32_t* (*)(std::uint32_t*, TESObjectREFR*);
		static REL::Relocation<resolve_body_t> resolveBody{ ID::Spaceship::Travel::ResolveReferenceBody };
		resolveBody(std::addressof(bodyID), a_reference);
		if (!bodyID) {
			return 0;
		}
		std::uint32_t orbitID{};
		using resolve_orbit_t = std::uint32_t* (*)(std::uint32_t*, std::uint32_t);
		static REL::Relocation<resolve_orbit_t> resolveOrbit{ ID::Spaceship::Travel::BodyToOrbit };
		resolveOrbit(std::addressof(orbitID), bodyID);
		return orbitID;
	}

	std::uint32_t GetBodySystemID(std::uint32_t a_bodyID)
	{
		std::uint32_t systemID{};
		using func_t = std::uint32_t* (*)(std::uint32_t*, std::uint32_t);
		static REL::Relocation<func_t> func{ ID::Spaceship::Travel::BodyToSystem };
		func(std::addressof(systemID), a_bodyID);
		return systemID;
	}

	CelestialBodyType GetBodyType(std::uint32_t a_bodyID)
	{
		using func_t = std::uint8_t (*)(std::uint32_t);
		static REL::Relocation<func_t> func{ ID::Spaceship::Travel::GetBodyType };
		return static_cast<CelestialBodyType>(func(a_bodyID));
	}

	BSTArray<CelestialBodyInfo> GetOtherBodiesInSystem(std::uint32_t a_referenceBodyID)
	{
		BSTArray<CelestialBodyInfo> bodies;
		static const BodyCollectorDispatch dispatch{ {}, CollectBody };
		BodyCollector collector{ std::addressof(dispatch), GetBodySystemID(a_referenceBodyID), std::addressof(bodies) };
		VisitBodies(a_referenceBodyID, false, std::addressof(collector));
		return bodies;
	}

	BSContainer::ForEachResult ForEachSpaceReference(TESObjectREFR* a_source, ReferenceCallback a_callback)
	{
		static const BodyCollectorDispatch dispatch{ {}, CollectReference };
		ReferenceCollector collector{ std::addressof(dispatch), std::addressof(a_callback) };
		return VisitBodies(a_source->GetFormID(), true, std::addressof(collector));
	}
}
