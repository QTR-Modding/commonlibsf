#include "RE/B/BGSGenericBaseForm.h"
#include "BlueprintCellReadScope.h"

#include "RE/P/PlayerKnowledge.h"
#include "RE/T/TESObjectCELL.h"

#include <atomic>

RE::BlueprintCellReadScope::BlueprintCellReadScope(std::uintptr_t a_database) :
	_owner(*reinterpret_cast<std::byte**>(a_database + 0x238))
{
	std::atomic_ref count{ *reinterpret_cast<std::uint32_t*>(_owner + 0x8) };
	count.fetch_add(1);
	using func_t = std::int32_t* (*)(void*);
	static REL::Relocation<func_t> acquire{ ID::BSComponentDB2::AcquireRead };
	_readCount = acquire(_owner + 0x10);
}

RE::BlueprintCellReadScope::~BlueprintCellReadScope()
{
	if (--*_readCount == 0) {
		std::atomic_ref completed{ *reinterpret_cast<std::uint32_t*>(_owner + 0x21C) };
		completed.fetch_add(4);
	}
	std::atomic_ref count{ *reinterpret_cast<std::uint32_t*>(_owner + 0x8) };
	if (count.fetch_sub(1) == 1) {
		using destructor_t = void (*)(void*, std::uint32_t);
		const auto table = *reinterpret_cast<std::uintptr_t**>(_owner);
		reinterpret_cast<destructor_t>(table[0])(_owner, 1);
	}
}

std::vector<RE::NiPointer<RE::TESObjectCELL>> RE::BGSGenericBaseForm::GetBlueprintCells() const
{
	struct Edge
	{
		std::uint64_t first;
		std::uint64_t second;
	};
	struct Cursor
	{
		const Edge*   page{};
		std::uint64_t index{};
	};
	using lookup_t = Cursor* (*)(const std::uintptr_t*, Cursor*, void*, const Edge*);
	static REL::Relocation<lookup_t> lookup{ ID::BSComponentDB2::LowerBoundEdge };
	static REL::Relocation<const std::uint16_t*> relation{ ID::BGSGenericBaseForm::BlueprintCellOwner };

	std::vector<NiPointer<TESObjectCELL>> cells;
	const auto database = BSGalaxy::GetKnowledgeDB();
	if (!database) {
		return cells;
	}
	BlueprintCellReadScope scope{ database };
	Edge key{ (static_cast<std::uint64_t>(GetFormID()) << 32) | *relation, 0 };
	while (true) {
		Cursor cursor;
		lookup(std::addressof(database), std::addressof(cursor), nullptr, std::addressof(key));
		if (!cursor.page) {
			break;
		}
		const Edge edge = cursor.page[cursor.index + 2];
		if (edge.first != key.first) {
			break;
		}
		TESObjectCELL* cell = TESForm::LookupByID<TESObjectCELL>(static_cast<TESFormID>(edge.second));
		if (cell) {
			cells.emplace_back(cell);
		}
		key.second = edge.second + 1;
	}
	return cells;
}

bool RE::BGSGenericBaseForm::IsSpaceshipBase() const
{
	using func_t = decltype(&BGSGenericBaseForm::IsSpaceshipBase);
	static REL::Relocation<func_t> func{ ID::BGSGenericBaseForm::IsSpaceshipBase };
	return func(this);
}
