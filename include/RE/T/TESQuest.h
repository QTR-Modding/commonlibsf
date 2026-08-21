#pragma once

#include "RE/B/BGSStoryManagerTreeForm.h"

#include <memory>
#include <type_traits>

namespace RE
{
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
		std::byte                                 pad074[0x94];    // 074
		QUEST_DATA                                data;            // 108
		std::byte                                 pad110[0x4];     // 110
		REX::TEnumSet<RuntimeFlag, std::uint32_t> runtimeFlags;    // 114
		std::byte                                 pad118[0x218];   // 118
		BGSStoryEvent*                            startEventData;  // 330
	};
	static_assert(offsetof(TESQuest, instanceID) == 0x70);
	static_assert(offsetof(TESQuest, runtimeFlags) == 0x114);
	static_assert(offsetof(TESQuest, startEventData) == 0x330);
}
