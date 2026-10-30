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
