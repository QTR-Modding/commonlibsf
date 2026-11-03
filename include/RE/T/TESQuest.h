#pragma once

#include "RE/B/BGSStoryManagerTreeForm.h"
#include "RE/B/BSPointerHandle.h"
#include "RE/B/BSTArray.h"

#include <memory>
#include <type_traits>
#include <vector>

namespace RE
{
	class Actor;
	class BGSBaseAlias;
	class BGSStoryEvent;

	struct QUEST_DATA
	{
		// members
		float         questDelayTime;  // 0
		std::uint16_t flags;           // 4
		std::int8_t   priority;        // 6
		std::uint8_t  questType;       // 7
	};
	static_assert(sizeof(QUEST_DATA) == 0x8);

	struct QuestInstanceKey
	{
		TESFormID     formID;      // 0
		std::uint32_t instanceID;  // 4

		[[nodiscard]] constexpr bool operator==(const QuestInstanceKey&) const noexcept = default;
	};
	static_assert(sizeof(QuestInstanceKey) == 0x8);
	static_assert(std::is_trivially_copyable_v<QuestInstanceKey>);

	class __declspec(novtable) TESQuest :
		public BGSStoryManagerTreeForm
	{
	public:
		SF_RTTI_VTABLE(TESQuest);
		SF_FORMTYPE(QUST);

		enum class RuntimeFlag : std::uint32_t
		{
			kRunning = 1 << 0,
			kStopped = 1 << 1,
			kTracked = 1 << 11
		};

		~TESQuest() override;  // 00

		[[nodiscard]] bool IsStageDone(std::uint16_t a_stage) const
		{
			using func_t = decltype(&TESQuest::IsStageDone);
			static REL::Relocation<func_t> func{ ID::TESQuest::IsStageDone };
			return func(this, a_stage);
		}

		void FillAliasReference(std::int32_t a_aliasID, TESObjectREFR* a_reference, std::int32_t a_fillType)
		{
			using func_t = decltype(&TESQuest::FillAliasReference);
			static REL::Relocation<func_t> func{ ID::TESQuest::FillAliasReference };
			func(this, a_aliasID, a_reference, a_fillType);
		}

		[[nodiscard]] BSPointerHandle<TESObjectREFR> GetAliasReferenceHandle(std::int32_t a_aliasID)
		{
			using func_t = BSPointerHandle<TESObjectREFR>* (*)(TESQuest*, BSPointerHandle<TESObjectREFR>*, std::int32_t);
			static REL::Relocation<func_t> func{ ID::TESQuest::GetAliasReferenceHandle };
			BSPointerHandle<TESObjectREFR> result;
			func(this, std::addressof(result), a_aliasID);
			return result;
		}

		// Requests startup without waiting for quest initialization. A true result
		// means the request was accepted, not that aliases or scripts are ready.
		// Call on the game thread and observe native quest initialization events.
		[[nodiscard]] bool StartNoWait();

		// Game-thread access to authored aliases. Missing IDs return nullptr.
		[[nodiscard]] BGSBaseAlias* GetAlias(std::int32_t a_aliasID) const;
		// The quest must be running; use an owned, optional alias. Native clear
		// reasons match ReferenceAlias.Clear (12) and RefCollectionAlias.RemoveAll (16).
		void ClearAlias(std::int32_t a_aliasID, std::int32_t a_clearType);
		// The ID must name a reference collection alias on this running quest.
		// Reference pointers must be non-null. These update native alias packages.
		void AddAliasReference(std::int32_t a_aliasID, TESObjectREFR* a_reference);
		void RemoveAliasReference(std::int32_t a_aliasID, TESObjectREFR* a_reference);
		// The ID must name a reference collection alias on this running quest.
		// Copies its resolved actors in native order.
		// Call on the game thread; returned references own their lifetimes.
		[[nodiscard]] std::vector<NiPointer<Actor>> GetAliasActors(std::int32_t a_aliasID);

		[[nodiscard]] constexpr std::uint32_t GetInstanceID() const noexcept { return instanceID; }

		[[nodiscard]] QuestInstanceKey GetInstanceKey() const noexcept
		{
			return { GetFormID(), GetInstanceID() };
		}

		[[nodiscard]] constexpr bool IsRunning() const noexcept { return runtimeFlags.all(RuntimeFlag::kRunning); }
		[[nodiscard]] constexpr bool IsStopped() const noexcept { return runtimeFlags.all(RuntimeFlag::kStopped); }
		[[nodiscard]] constexpr bool IsTracked() const noexcept { return runtimeFlags.all(RuntimeFlag::kTracked); }

		// This is the engine's toggle operation. Callers that need set semantics
		// must compare IsTracked() with the desired state before invoking it.
		void ToggleTracking()
		{
			const auto key = GetInstanceKey();
			using func_t = void (*)(const QuestInstanceKey*);
			static REL::Relocation<func_t> func{ ID::TESQuest::ToggleTracking };
			func(std::addressof(key));
		}

		// members
		std::byte                                 pad038[0x38];    // 038
		std::uint32_t                             instanceID;      // 070
		std::byte                                 pad074[0x14];    // 074
		BSTArray<BGSBaseAlias*>                   aliases;         // 088
		std::byte                                 pad098[0x40];    // 098
		std::byte*                                aliasInstances;  // 0D8
		std::byte                                 pad0E0[0x28];    // 0E0
		QUEST_DATA                                data;            // 108
		std::byte                                 pad110[0x4];     // 110
		REX::TEnumSet<RuntimeFlag, std::uint32_t> runtimeFlags;    // 114
		std::byte                                 pad118[0x218];   // 118
		BGSStoryEvent*                            startEventData;  // 330
	};
	static_assert(offsetof(TESQuest, instanceID) == 0x70);
	static_assert(offsetof(TESQuest, aliases) == 0x88);
	static_assert(offsetof(TESQuest, aliasInstances) == 0xD8);
	static_assert(offsetof(TESQuest, runtimeFlags) == 0x114);
	static_assert(offsetof(TESQuest, startEventData) == 0x330);
}
