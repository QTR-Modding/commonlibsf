#include "RE/S/SpaceshipTravel.h"

#include "RE/B/BSContainer.h"

namespace RE::Spaceship::Travel
{
	namespace
	{
		struct BodyCollector;
		using BodyCallback = BSContainer::ForEachResult (*)(BodyCollector*, std::uint32_t, const void*, const void*, const void*);

		// The synchronous iterator reads only target.vtable[2], never the two
		// preceding slots. It neither copies, destroys nor retains this target.
		// Borrowed BodyDrawData/reference arguments remain opaque to the collector.
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

		BSContainer::ForEachResult CollectBody(BodyCollector* a_collector, std::uint32_t a_bodyID, const void*, const void*, const void*)
		{
			const auto systemID = GetBodySystemID(a_bodyID);
			if (systemID == a_collector->systemID) {
				BSFixedString name;
				using func_t = BSFixedString* (*)(BSFixedString*, std::uint32_t);
				static REL::Relocation<func_t> func{ ID::Spaceship::Travel::GetCelestialBodyName };
				func(std::addressof(name), a_bodyID);
				a_collector->bodies->push_back({ { systemID, a_bodyID }, std::move(name), GetBodyType(a_bodyID) });
			}
			return BSContainer::ForEachResult::kContinue;
		}
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
		using func_t = BSContainer::ForEachResult (*)(std::uint32_t, bool, BodyCollector*);
		static REL::Relocation<func_t> func{ ID::Spaceship::Travel::ForEachCelestialBody };
		func(a_referenceBodyID, false, std::addressof(collector));
		return bodies;
	}
}
