#pragma once

#include "RE/B/BSContainer.h"
#include "RE/B/BSTEvent.h"
#include "RE/N/NiSmartPointer.h"

namespace RE
{
	namespace BSResource::Archive2
	{
		class StreamOpenedEvent;
	}

	class BGSLocation;
	class NiPoint3;
	class Sky;
	class TESActorBase;
	class TESObjectCELL;
	class TESObjectREFR;
	class TESWorldSpace;

	struct PositionPlayerEvent;

	class __declspec(novtable) TES :
		public BSTEventSink<BSResource::Archive2::StreamOpenedEvent>,  // 000
		public BSTEventSink<PositionPlayerEvent>                       // 008
	{
	public:
		SF_RTTI_VTABLE(TES);

		virtual ~TES();  // 00

		// override (BSTEventSink<BSResource::Archive2::StreamOpenedEvent>)
		BSEventNotifyControl ProcessEvent(const BSResource::Archive2::StreamOpenedEvent& a_event, BSTEventSource<BSResource::Archive2::StreamOpenedEvent>* a_eventSource) override;  // 01 - { return BSEventNotifyControl::kContinue; }

		// override (BSTEventSink<PositionPlayerEvent>)
		BSEventNotifyControl ProcessEvent(const PositionPlayerEvent& a_event, BSTEventSource<PositionPlayerEvent>* a_eventSource) override;  // 01

		[[nodiscard]] static TES* GetSingleton()
		{
			static REL::Relocation<TES**> singleton{ ID::TES::Singleton };
			return *singleton;
		}

		std::uint16_t GetDeadCount(TESActorBase* a_actorBase)
		{
			using func_t = decltype(&TES::GetDeadCount);
			static REL::Relocation<func_t> func{ ID::TES::GetDeadCount };
			return func(this, a_actorBase);
		}

		// Visits the native attached-cell snapshot synchronously. References are borrowed
		// under the cell read lock; copy a NiPointer to retain one, and do not change cell membership.
		BSContainer::ForEachResult ForEachReference(std::function<BSContainer::ForEachResult(const NiPointer<TESObjectREFR>&)> a_callback);

		// Selects the shared primary world and invalidates its exterior grid.
		void SetWorldSpace(TESWorldSpace* a_worldSpace);

		// Game-thread loading lifecycle. Begin and Finish belong to the same caller.
		// Begin requires no active transition; finish only this caller's loading work.
		static void BeginSceneLoad(BGSLocation* a_location = nullptr);
		[[nodiscard]] static bool IsSceneLoading();
		static void FinishSceneLoad();

		// Native scene-loading operations. Call on the game thread within a coordinated
		// world transition; they change shared world state and own their grid-mutation scope.
		// The caller owns presentation, actor state, continuing focus and restoration.
		static void LoadExterior(const NiPoint3& a_position);
		// A forced transition bypasses the same-cell focus-only path.
		static void LoadInterior(TESObjectCELL* a_cell, const NiPoint3& a_position, bool a_forceTransition = false);

		// Submits the ordinary deferred exterior focus update. This is not a load-completion receipt.
		void SubmitExteriorFocus(const NiPoint3& a_position);

		// members
		std::byte      pad010[216];   // 010
		TESObjectCELL* interiorCell;  // 0E8
		std::byte      pad0F0[80];    // 0F0
		Sky*           sky;           // 140
		std::byte      pad148[64];    // 148
		TESWorldSpace* worldSpace;    // 188
		void*          unk190;        // 190
	};
	static_assert(offsetof(TES, sky) == 0x140);
	static_assert(offsetof(TES, interiorCell) == 0xE8);
}
