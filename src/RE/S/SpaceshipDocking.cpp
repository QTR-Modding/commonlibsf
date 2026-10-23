#include "RE/S/SpaceshipDocking.h"

#include "RE/B/BSTArray.h"
#include "RE/T/TESIdleForm.h"

namespace RE::Spaceship::Docking
{
	namespace
	{
		static_assert(offsetof(TESIdleForm, conditions) == 0x48);

		// The docking caller initializes only these two references and zeroes the remaining context.
		// Keep the unresolved native tail private instead of reusing Script.h's different layout.
		struct DockConditionContext
		{
			TESObjectREFR* source;
			TESObjectREFR* target;
			std::byte     unk10[0x60]{};
		};
		static_assert(sizeof(DockConditionContext) == 0x70);

		[[nodiscard]] bool ConditionsPass(const TESCondition& a_conditions, DockConditionContext& a_context)
		{
			using func_t = bool (*)(const TESCondition*, DockConditionContext*);
			static REL::Relocation<func_t> func{ ID::TESCondition::IsTrue };
			return func(std::addressof(a_conditions), std::addressof(a_context));
		}
	}

	TESObjectREFR* FindPort(TESObjectREFR* a_source, TESObjectREFR* a_target)
	{
		using func_t = decltype(&FindPort);
		static REL::Relocation<func_t> func{ ID::Spaceship::Docking::FindPort };
		return func(a_source, a_target);
	}

	bool GetStartLocations(TESObjectREFR* a_source, TESObjectREFR* a_targetPort, BSScrapArray<InteractionGroup>& a_out)
	{
		using func_t = bool (*)(TESObjectREFR*, void*, std::int32_t, bool, bool, void*, TESObjectREFR*, BSScrapArray<InteractionGroup>&, bool, bool, bool);
		static REL::Relocation<func_t> func{ ID::Spaceship::Docking::GetInteractionLocations };
		a_out.clear();
		return func(a_source, nullptr, -1, false, false, nullptr, a_targetPort, a_out, true, true, true);
	}

	bool GetStartPose(TESObjectREFR* a_source, TESObjectREFR* a_targetPort, StartPose& a_out)
	{
		BSScrapArray<InteractionGroup> groups;
		if (!GetStartLocations(a_source, a_targetPort, groups) || groups.empty() || groups.front().entries.empty()) {
			return false;
		}

		const auto& entries = groups.front().entries;
		const auto* selected = std::addressof(entries.front());
		for (const auto& entry : entries) {
			if (entry.idle) {
				DockConditionContext context{ a_source, a_targetPort };
				if (ConditionsPass(entry.idle->conditions, context)) {
					selected = std::addressof(entry);
					break;
				}
			}
		}

		a_out.rotation = selected->rotation;
		a_out.translation = selected->translation;
		return true;
	}

	bool BeginDock(TESObjectREFR* a_source, TESObjectREFR* a_target, TESObjectREFR* a_targetPort)
	{
		using func_t = decltype(&BeginDock);
		static REL::Relocation<func_t> func{ ID::Spaceship::Docking::BeginDock };
		return func(a_source, a_target, a_targetPort);
	}
}
