#include "RE/S/SpaceshipTravel.h"

#include "RE/S/ScaleformGFxValue.h"
#include "RE/T/TESObjectREFR.h"

namespace RE::Spaceship::Travel
{
	namespace
	{
		struct LandingRows
		{
			LandingRows() = default;
			LandingRows(const LandingRows&) = delete;
			LandingRows& operator=(const LandingRows&) = delete;

			~LandingRows()
			{
				using func_t = void (*)(LandingRows*);
				static REL::Relocation<func_t> func{ ID::Spaceship::Travel::DestroyLandingRows };
				func(this);
			}

			std::byte* begin{};
			std::byte* end{};
			std::byte* capacityEnd{};
		};
		static_assert(sizeof(LandingRows) == 0x18);
		static_assert(offsetof(LandingRows, end) == 0x08);
		static_assert(offsetof(LandingRows, capacityEnd) == 0x10);

		struct LandingCollectorContext
		{
			const std::uint32_t* bodyID;
			LandingRows* rows;
			bool differentBody{};
		};
		static_assert(offsetof(LandingCollectorContext, rows) == 0x08);
		static_assert(offsetof(LandingCollectorContext, differentBody) == 0x10);

		// The stock closure occupies the first 0x10 bytes of the native function's
		// 0x20-byte inline storage. Its target pointer is at 0x20. The stock closure
		// borrows the context; its inline destructor has no work to perform.
		struct AuthoredWorldCollector
		{
			const void* vtable;
			LandingCollectorContext* context;
			std::byte inlineStorageTail[0x10]{};
			AuthoredWorldCollector* target{ this };
		};
		static_assert(offsetof(AuthoredWorldCollector, context) == 0x08);
		static_assert(offsetof(AuthoredWorldCollector, target) == 0x20);
		static_assert(sizeof(AuthoredWorldCollector) == 0x28);

		struct LandingSiteSelection
		{
			SurfaceCoordinates coordinates;
			bool cleared;
			bool validPoint;
		};

		template <class T>
		[[nodiscard]] T ReadRowField(const std::byte* a_row, std::size_t a_offset)
		{
			T value;
			std::memcpy(std::addressof(value), a_row + a_offset, sizeof(value));
			return value;
		}

		void CollectLandingRows(std::uint32_t a_bodyID, LandingRows& a_rows)
		{
			LandingCollectorContext context{ std::addressof(a_bodyID), std::addressof(a_rows) };
			static REL::Relocation<const void*> collectorVtable{ ID::Spaceship::Travel::StockWorldCollectorVtable };
			AuthoredWorldCollector collector{ collectorVtable.get(), std::addressof(context) };
			using authored_t = void (*)(std::uint32_t, AuthoredWorldCollector*);
			static REL::Relocation<authored_t> authored{ ID::Spaceship::Travel::CollectAuthoredWorlds };
			authored(a_bodyID, std::addressof(collector));

			using runtime_t = void* (*)();
			static REL::Relocation<runtime_t> runtime{ ID::BSGalaxy::GetKnowledgeManager };
			void* database;
			std::memcpy(std::addressof(database), static_cast<const std::byte*>(runtime()) + 0x8B0, sizeof(database));
			using registered_t = void (*)(void*, void*, const std::uint32_t*, LandingCollectorContext*);
			static REL::Relocation<registered_t> registered{ ID::Spaceship::Travel::CollectRegisteredWorlds };
			registered(database, nullptr, std::addressof(a_bodyID), std::addressof(context));

			using extra_t = void (*)(const std::uint32_t*, LandingRows*);
			static REL::Relocation<extra_t> outposts{ ID::Spaceship::Travel::AddPlayerOutposts };
			static REL::Relocation<extra_t> currentSite{ ID::Spaceship::Travel::AddCurrentSite };
			outposts(std::addressof(a_bodyID), std::addressof(a_rows));
			currentSite(std::addressof(a_bodyID), std::addressof(a_rows));
		}

		[[nodiscard]] std::optional<LandingSiteSelection> FindLandingSite(std::uint32_t a_bodyID, std::uint32_t a_marker)
		{
			LandingRows rows;
			CollectLandingRows(a_bodyID, rows);

			// BodyInspect rows own three strings and the native vector allocation.
			for (auto row = rows.begin; row != rows.end; row += 0x80) {
				if (ReadRowField<std::uint32_t>(row, 0x00) != a_marker) {
					continue;
				}
				// Type 4 invokes the separate foot-to-ship fast-travel action.
				if (ReadRowField<std::uint8_t>(row, 0x68) == 4) {
					return std::nullopt;
				}
				return LandingSiteSelection{
					{ ReadRowField<double>(row, 0x48), ReadRowField<double>(row, 0x40) },
					ReadRowField<std::uint8_t>(row, 0x6E) != 0,
					ReadRowField<std::uint8_t>(row, 0x6D) != 0
				};
			}
			return std::nullopt;
		}

		struct NativeSmallError
		{
			~NativeSmallError()
			{
				if (capacity > sizeof(storage)) {
					void* allocation;
					std::memcpy(std::addressof(allocation), storage, sizeof(allocation));
					using func_t = void (*)(void*);
					static REL::Relocation<func_t> func{ ID::Spaceship::Travel::FreeNativeSmallStringHeap };
					func(allocation);
				}
			}

			[[nodiscard]] const char* c_str() const
			{
				if (capacity <= sizeof(storage)) {
					return storage;
				}
				const char* allocation;
				std::memcpy(std::addressof(allocation), storage, sizeof(allocation));
				return allocation;
			}

			alignas(void*) char storage[12]{};
			std::uint16_t capacity{ 1 };
			std::uint16_t length{};
		};
		static_assert(offsetof(NativeSmallError, capacity) == 0x0C);
		static_assert(offsetof(NativeSmallError, length) == 0x0E);
		static_assert(sizeof(NativeSmallError) == 0x10);

		// The native availability method reads these four selection flags from
		// the JumpData model; the UI-only tail is not needed for this query.
		struct LandingAvailabilityModelPrefix
		{
			Scaleform::GFx::Value value;
			BSFixedString name;
			BSFixedString description;
			bool shipFits;
			bool cleared;
			bool travelAllowed;
			bool validPoint;
		};
		static_assert(offsetof(LandingAvailabilityModelPrefix, name) == 0x30);
		static_assert(offsetof(LandingAvailabilityModelPrefix, description) == 0x38);
		static_assert(offsetof(LandingAvailabilityModelPrefix, shipFits) == 0x40);
		static_assert(offsetof(LandingAvailabilityModelPrefix, cleared) == 0x41);
		static_assert(offsetof(LandingAvailabilityModelPrefix, travelAllowed) == 0x42);
		static_assert(offsetof(LandingAvailabilityModelPrefix, validPoint) == 0x43);

		void InitializeBodyKnowledge(std::uint32_t a_bodyID)
		{
			using get_t = std::int32_t (*)(std::uint32_t);
			static REL::Relocation<get_t> get{ ID::Spaceship::Travel::GetBodyKnowledge };
			if (get(a_bodyID) == -1) {
				using scanner_t = std::int32_t (*)();
				static REL::Relocation<scanner_t> scanner{ ID::Spaceship::Travel::GetPlayerScannerLevel };
				using set_t = void (*)(std::uint32_t, std::int32_t, bool);
				static REL::Relocation<set_t> set{ ID::BGSPlanet::ScanLevelWriter };
				set(a_bodyID, scanner(), false);
			}
		}
	}

	BSTArray<LandingSiteInfo> GetCurrentLandingSites()
	{
		LandingRows rows;
		CollectLandingRows(GetCurrentBodyID(), rows);
		BSTArray<LandingSiteInfo> sites;
		for (auto row = rows.begin; row != rows.end; row += 0x80) {
			const auto handle = ReadRowField<std::uint32_t>(row, 0x00);
			if (handle == 0 || ReadRowField<std::uint8_t>(row, 0x68) == 4) {
				continue;
			}
			// Copy the native BSFixedStrings before the row owner releases them.
			sites.push_back({
				BSPointerHandle<TESObjectREFR>{ handle },
				{ ReadRowField<double>(row, 0x48), ReadRowField<double>(row, 0x40) },
				*reinterpret_cast<const BSFixedString*>(row + 0x08),
				*reinterpret_cast<const BSFixedString*>(row + 0x10),
				*reinterpret_cast<const BSFixedString*>(row + 0x18),
				ReadRowField<std::uint8_t>(row, 0x6C) != 0
			});
		}
		return sites;
	}

	LandingRequestResult RequestLandingAtMarker(std::uint32_t a_bodyID, BSPointerHandle<TESObjectREFR> a_marker)
	{
		if (GetCurrentBodyID() != a_bodyID) {
			return { LandingRequestStatus::kNotAtBody, {} };
		}
		if (!a_marker) {
			return { LandingRequestStatus::kMarkerUnavailable, {} };
		}
		const auto site = FindLandingSite(a_bodyID, a_marker.get_handle());
		if (!site) {
			return { LandingRequestStatus::kMarkerUnavailable, {} };
		}

		NativeSmallError error;
		using admission_t = bool (*)(std::uint32_t, bool, bool, NativeSmallError*);
		static REL::Relocation<admission_t> admission{ ID::Spaceship::Travel::SharedLandingAdmission };
		if (!admission(a_bodyID, site->cleared, true, std::addressof(error))) {
			return { LandingRequestStatus::kDenied, BSFixedString{ error.c_str() } };
		}

		using ship_t = NiPointer<TESObjectREFR>* (*)(NiPointer<TESObjectREFR>*);
		static REL::Relocation<ship_t> sourceShip{ ID::Spaceship::Travel::GetLandingSourceShip };
		NiPointer<TESObjectREFR> ship;
		sourceShip(std::addressof(ship));
		using fits_t = bool (*)(TESObjectREFR*);
		static REL::Relocation<fits_t> fits{ ID::Spaceship::Travel::ShipFits };
		using location_t = bool (*)(std::uint32_t, bool);
		static REL::Relocation<location_t> location{ ID::Spaceship::Travel::LocationAdmission };
		LandingAvailabilityModelPrefix model{
			{}, {}, {}, !ship || fits(ship.get()), site->cleared,
			location(a_bodyID, true), site->validPoint
		};
		BSFixedString denial;
		if (!location(a_bodyID, false)) {
			return { LandingRequestStatus::kDenied, BSFixedString{ "$Unable to Jump" } };
		}
		using available_t = bool (*)(const LandingAvailabilityModelPrefix*, BSFixedString*);
		static REL::Relocation<available_t> available{ ID::Spaceship::Travel::CheckLandingAvailability };
		if (!available(std::addressof(model), std::addressof(denial))) {
			return { LandingRequestStatus::kDenied, std::move(denial) };
		}

		RequestLanding(a_bodyID, site->coordinates, a_marker, ShouldPlayLandingAnimation(a_marker, a_bodyID));
		InitializeBodyKnowledge(a_bodyID);
		return { LandingRequestStatus::kRequested, {} };
	}
}
