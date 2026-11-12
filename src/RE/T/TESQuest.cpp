#include "RE/T/TESQuest.h"

#include "RE/A/Actor.h"
#include "RE/G/GameVM.h"

#include <limits>

namespace RE
{
	namespace
	{
		std::uint32_t GetAliasIndex(const TESQuest* a_quest, std::int32_t a_aliasID)
		{
			using func_t = std::uint32_t (*)(const TESQuest*, std::int32_t);
			static REL::Relocation<func_t> func{ ID::TESQuest::GetAliasIndex };
			return func(a_quest, a_aliasID);
		}
	}

	bool TESQuest::StartNoWait()
	{
		auto* game = GameVM::GetSingleton();
		auto* vm = game ? game->GetVM() : nullptr;
		if (!vm) {
			return false;
		}
		using func_t = bool (*)(BSScript::IVirtualMachine*, std::uint32_t, TESQuest*);
		static REL::Relocation<func_t> func{ ID::TESQuest::StartNoWait };
		return func(vm, 0, this);
	}

	BGSBaseAlias* TESQuest::GetAlias(std::int32_t a_aliasID) const
	{
		const auto index = GetAliasIndex(this, a_aliasID);
		return index != std::numeric_limits<std::uint32_t>::max() ? aliases[index] : nullptr;
	}

	void TESQuest::ClearAlias(std::int32_t a_aliasID, std::int32_t a_clearType)
	{
		if (auto* alias = GetAlias(a_aliasID)) {
			using func_t = void (*)(TESQuest*, BGSBaseAlias*, std::int32_t);
			static REL::Relocation<func_t> func{ ID::TESQuest::ClearAlias };
			func(this, alias, a_clearType);
		}
	}

	void TESQuest::AddAliasReference(std::int32_t a_aliasID, TESObjectREFR* a_reference)
	{
		if (auto* alias = GetAlias(a_aliasID)) {
			using func_t = void (*)(TESQuest*, BGSBaseAlias*, TESObjectREFR*);
			static REL::Relocation<func_t> func{ ID::TESQuest::AddAliasReference };
			func(this, alias, a_reference);
		}
	}

	void TESQuest::RemoveAliasReference(std::int32_t a_aliasID, TESObjectREFR* a_reference)
	{
		if (auto* alias = GetAlias(a_aliasID)) {
			using func_t = void (*)(TESQuest*, BGSBaseAlias*, TESObjectREFR*);
			static REL::Relocation<func_t> func{ ID::TESQuest::RemoveAliasReference };
			func(this, alias, a_reference);
		}
	}

	std::vector<NiPointer<Actor>> TESQuest::GetAliasActors(std::int32_t a_aliasID)
	{
		std::vector<NiPointer<Actor>> result;
		const auto                    index = GetAliasIndex(this, a_aliasID);
		if (index != std::numeric_limits<std::uint32_t>::max()) {
			BSScrapArray<Actor*> actors;
			auto*                output = std::addressof(actors);
			using func_t = std::uint64_t (*)(void*, BSScrapArray<Actor*>**);
			static REL::Relocation<func_t> func{ ID::TESQuest::CollectAliasActors };
			// Runtime alias entries are 24-byte small arrays of reference handles.
			func(aliasInstances + index * 0x18, std::addressof(output));
			result.reserve(actors.size());
			for (auto* actor : actors) {
				result.emplace_back(actor);
			}
		}
		return result;
	}
}
