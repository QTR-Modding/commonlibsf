#include "RE/T/TES.h"
#include "RE/T/TESObjectCELL.h"

namespace RE
{
	namespace
	{
		using ReferenceCallback = std::function<BSContainer::ForEachResult(const NiPointer<TESObjectREFR>&)>;

		// The native walkers borrow these callback objects and invoke only the named slots.
		// Unused vtable slots are opaque; no destruction or retention crosses this boundary.
		struct ReferenceVisitor
		{
			struct VTable
			{
				std::uintptr_t opaque00;
				std::uintptr_t opaque08;
				BSContainer::ForEachResult (*invoke)(ReferenceVisitor*, const NiPointer<TESObjectREFR>&);
			};

			static BSContainer::ForEachResult Invoke(ReferenceVisitor* a_self, const NiPointer<TESObjectREFR>& a_reference)
			{
				return (*a_self->callback)(a_reference);
			}

			const VTable*            vtable;
			const ReferenceCallback* callback;
		};
		static_assert(std::is_standard_layout_v<ReferenceVisitor>);
		static_assert(offsetof(ReferenceVisitor, vtable) == 0x00);
		static_assert(offsetof(ReferenceVisitor::VTable, invoke) == 0x10);

		BSContainer::ForEachResult VisitCell(const TESObjectCELL* a_cell, ReferenceVisitor* a_references)
		{
			using func_t = BSContainer::ForEachResult (*)(const TESObjectCELL*, ReferenceVisitor*, bool);
			static REL::Relocation<func_t> func{ ID::TESObjectCELL::ForEachReference };
			return func(a_cell, a_references, false);
		}

		struct CellVisitor
		{
			struct VTable
			{
				std::uintptr_t opaque00;
				BSContainer::ForEachResult (*visit)(CellVisitor*, TESObjectCELL*);
			};

			static BSContainer::ForEachResult Visit(CellVisitor* a_self, TESObjectCELL* a_cell)
			{
				return VisitCell(a_cell, a_self->references);
			}

			const VTable*     vtable;
			ReferenceVisitor* references;
		};
		static_assert(std::is_standard_layout_v<CellVisitor>);
		static_assert(offsetof(CellVisitor, vtable) == 0x00);
		static_assert(offsetof(CellVisitor::VTable, visit) == 0x08);
		static_assert(sizeof(BSContainer::ForEachResult) == 0x04);

		constexpr ReferenceVisitor::VTable referenceVTable{ 0, 0, ReferenceVisitor::Invoke };
		constexpr CellVisitor::VTable      cellVTable{ 0, CellVisitor::Visit };
	}

	void TESObjectCELL::ForEachReference(ReferenceCallback a_callback) const
	{
		ReferenceVisitor visitor{ std::addressof(referenceVTable), std::addressof(a_callback) };
		(void)VisitCell(this, std::addressof(visitor));
	}

	BSContainer::ForEachResult TES::ForEachReference(ReferenceCallback a_callback)
	{
		ReferenceVisitor references{ std::addressof(referenceVTable), std::addressof(a_callback) };
		CellVisitor      cells{ std::addressof(cellVTable), std::addressof(references) };
		using func_t = BSContainer::ForEachResult (*)(TES*, CellVisitor*, TESObjectCELL*);
		static REL::Relocation<func_t> func{ ID::TES::ForEachCell };
		return func(this, std::addressof(cells), nullptr);
	}
}

void RE::TES::SetWorldSpace(TESWorldSpace* a_worldSpace)
{
	using func_t = decltype(&TES::SetWorldSpace);
	static REL::Relocation<func_t> func{ ID::TES::SetWorldSpace };
	func(this, a_worldSpace);
}

void RE::TES::LoadExterior(const NiPoint3& a_position)
{
	using func_t = void (*)(void*, const NiPoint3*);
	static REL::Relocation<func_t> func{ ID::TES::LoadExterior };
	func(nullptr, std::addressof(a_position));
}

void RE::TES::LoadInterior(TESObjectCELL* a_cell, const NiPoint3& a_position)
{
	using func_t = void (*)(void*, TESObjectCELL*, const NiPoint3*, bool);
	static REL::Relocation<func_t> func{ ID::TES::LoadInterior };
	func(nullptr, a_cell, std::addressof(a_position), false);
}

void RE::TES::SubmitExteriorFocus(const NiPoint3& a_position)
{
	using func_t = void (*)(TES*, const NiPoint3*, bool, bool, TESObjectCELL*);
	static REL::Relocation<func_t> func{ ID::TES::SubmitExteriorFocus };
	func(this, std::addressof(a_position), true, false, nullptr);
}

void RE::TES::BeginSceneLoad(BGSLocation* a_location)
{
	using func_t = void (*)(BGSLocation*, bool, std::uint32_t, bool, bool);
	static REL::Relocation<func_t> func{ ID::TES::BeginSceneLoad };
	func(a_location, false, 0, false, true);
}

bool RE::TES::IsSceneLoading()
{
	static REL::Relocation<const bool*> active{ ID::TES::SceneLoading };
	return *active;
}

void RE::TES::FinishSceneLoad()
{
	using func_t = void (*)(bool, BGSLocation*, bool, float, bool);
	static REL::Relocation<func_t> func{ ID::TES::FinishSceneLoad };
	func(true, nullptr, false, -1.0F, false);
}
