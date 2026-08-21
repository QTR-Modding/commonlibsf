#pragma once

#include "RE/B/BSCoreTypes.h"
#include "RE/B/BSFixedString.h"
#include "RE/B/BSTEvent.h"
#include "RE/B/BSTHeapSTLVector.h"
#include "RE/G/GameMenuBase.h"

namespace RE
{
	struct StarMap
	{
		enum class SurfaceMarkerType : std::uint16_t
		{
			kQuest = 0x48
		};

		struct PlanetTraitKnownEvent
		{
			[[nodiscard]] static BSTEventSource<PlanetTraitKnownEvent>* GetEventSource()
			{
				using func_t = decltype(&PlanetTraitKnownEvent::GetEventSource);
				static REL::Relocation<func_t> func{ ID::StarMap::PlanetTraitKnownEvent::GetEventSource };
				return func();
			}
		};

		struct SurfaceMarkerStaticData
		{
			[[nodiscard]] constexpr bool IsLocation() const noexcept { return isLocation == 1; }
			[[nodiscard]] constexpr bool HasQuestTarget() const noexcept { return hasQuestTarget == 1; }
			[[nodiscard]] constexpr bool IsQuestActive() const noexcept { return questActive == 1; }

			// members
			std::byte                       pad000[0x28];     // 00
			BSFixedString                   nameText;         // 28
			BSFixedString                   extraText;        // 30
			BSFixedString                   questTargetText;  // 38
			std::uint32_t                   markerHandleBits;  // 40
			std::uint32_t                   pad044;           // 44
			BSTHeapSTLVector<TESFormID>      questOwners;      // 48
			SurfaceMarkerType               markerType;       // 60
			std::byte                       pad062[0x6];       // 62
			std::uint8_t                    isLocation;       // 68
			std::uint8_t                    hasQuestTarget;   // 69
			std::uint8_t                    questActive;      // 6A
			std::byte                       pad06B[0x15];      // 6B
		};
		static_assert(offsetof(SurfaceMarkerStaticData, nameText) == 0x28);
		static_assert(offsetof(SurfaceMarkerStaticData, extraText) == 0x30);
		static_assert(offsetof(SurfaceMarkerStaticData, questTargetText) == 0x38);
		static_assert(offsetof(SurfaceMarkerStaticData, markerHandleBits) == 0x40);
		static_assert(offsetof(SurfaceMarkerStaticData, questOwners) == 0x48);
		static_assert(offsetof(SurfaceMarkerStaticData, markerType) == 0x60);
		static_assert(offsetof(SurfaceMarkerStaticData, isLocation) == 0x68);
		static_assert(offsetof(SurfaceMarkerStaticData, hasQuestTarget) == 0x69);
		static_assert(offsetof(SurfaceMarkerStaticData, questActive) == 0x6A);
		static_assert(sizeof(SurfaceMarkerStaticData) == 0x80);

		class SurfaceMapState
		{
		public:
			SF_RTTI_VTABLE(StarMap__SurfaceMapState);

			inline static constexpr REL::ID PRIMARY_VTABLE = RE::VTABLE::StarMap__SurfaceMapState[0];

			// Surface Map state is owned by the Star Map UI thread. These operations
			// are not thread-safe or reentrant.

			void RebuildSurfaceMarkers()
			{
				using func_t = void (*)(SurfaceMapState*);
				static REL::Relocation<func_t> func{ ID::StarMap::SurfaceMapState::RebuildSurfaceMarkers };
				func(this);
			}

			void Refresh()
			{
				using func_t = void (*)(SurfaceMapState*);
				static REL::Relocation<func_t> func{ ID::StarMap::SurfaceMapState::Refresh };
				func(this);
			}

			// members
			// Engine-owned storage. External callers must treat it as read-only.
			// Elements and their nested storage are invalidated by rebuild, refresh,
			// state transition, and destruction. Do not retain pointers or views.
			std::byte                                  pad000[0x8B8];  // 000
			BSTHeapSTLVector<SurfaceMarkerStaticData> surfaceMarkers;  // 8B8
		};
		static_assert(offsetof(SurfaceMapState, surfaceMarkers) == 0x8B8);

		class StarMapMenu :
			public GameMenuBase
		{
		public:
			SF_RTTI_VTABLE(StarMap__StarMapMenu);
			SF_MENU_NAME("GalaxyStarMapMenu");

			inline static constexpr REL::ID PRIMARY_VTABLE = RE::VTABLE::StarMap__StarMapMenu[12];
			inline static constexpr REL::ID BSINPUTEVENTUSER_VTABLE = RE::VTABLE::StarMap__StarMapMenu[13];

			void OnButtonEvent(const ButtonEvent* a_event) override
			{
				using func_t = void (*)(BSInputEventUser*, const ButtonEvent*);
				static REL::Relocation<func_t> func{ ID::StarMap::StarMapMenu::OnButtonEvent };
				func(this, a_event);
			}

			// Returns a non-owning owner-thread pointer. It is invalidated by Star Map
			// state transition and menu destruction; do not retain it.
			[[nodiscard]] SurfaceMapState* GetSurfaceMapState()
			{
				using func_t = SurfaceMapState* (*)(StarMapMenu*);
				static REL::Relocation<func_t> func{ ID::StarMap::StarMapMenu::GetSurfaceMapState };
				return func(this);
			}
		};
	};
}
