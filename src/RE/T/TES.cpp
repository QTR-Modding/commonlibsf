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

std::optional<float> RE::TES::GetGroundHeight(const NiPoint3& a_position, std::uint64_t a_collisionLayers, float a_startOffset, float a_distance) const
{
	using func_t = float (*)(const TES*, const NiPoint3&, std::uint64_t, float, float);
	static REL::Relocation<func_t> func{ ID::TES::GetGroundHeight };
	const float height = func(this, a_position, a_collisionLayers, a_startOffset, a_distance);
	if (height == std::numeric_limits<float>::lowest()) {
		return std::nullopt;
	}
	return height;
}

std::optional<std::uint64_t> RE::TES::QueueBoxCast(const NiPoint3& a_center, const NiPoint3& a_halfExtents, float a_startOffset, float a_distance) const
{
	struct Bounds
	{
		NiPoint3 center;
		NiPoint3 halfExtents;
	};
	static_assert(sizeof(Bounds) == 0x18);
	static_assert(offsetof(Bounds, halfExtents) == 0xC);

	const Bounds bounds{ a_center, a_halfExtents };
	constexpr std::uint64_t invalidHandle = std::numeric_limits<std::uint64_t>::max();
	std::uint64_t handle = invalidHandle;
	using func_t = void (*)(const TES*, std::uint64_t&, const Bounds&, std::uint8_t, float, float);
	static REL::Relocation<func_t> func{ ID::TES::QueueBoxCast };
	func(this, handle, bounds, 1, a_startOffset, a_distance);
	if (handle == invalidHandle) {
		return std::nullopt;
	}
	return handle;
}

RE::TES::BoxCastResult RE::TES::ReadBoxCast(std::uint64_t a_handle)
{
	struct Hit
	{
		std::byte unused[0x68];
		std::uint32_t queryType;
		std::byte tail[4];
	};
	struct ResultView
	{
		const Hit* hits{};
		std::uint32_t count{};
		std::uint32_t category{ 7 };
	};
	static_assert(sizeof(Hit) == 0x70);
	static_assert(offsetof(Hit, queryType) == 0x68);
	static_assert(sizeof(ResultView) == 0x10);
	static_assert(offsetof(ResultView, count) == 0x8);

	using func_t = std::uint32_t (*)(const std::uint64_t&, ResultView&, void*, std::uintptr_t);
	static REL::Relocation<func_t> func{ ID::CollisionQuery::ReadResult };
	static REL::Relocation<BSReadWriteLock*> lock{ ID::CollisionQuery::Lock };
	const BSAutoWriteLock locked{ lock.get() };
	ResultView result;
	switch (func(a_handle, result, nullptr, 0)) {
	case 1:
		if (result.count != 1 || !result.hits) {
			return BoxCastResult::kInvalid;
		}
		return result.hits->queryType == 0 ? BoxCastResult::kClear : BoxCastResult::kHit;
	case 2:
		return BoxCastResult::kPending;
	default:
		return BoxCastResult::kInvalid;
	}
}

bool RE::TES::CancelBoxCast(std::uint64_t a_handle)
{
	using func_t = bool (*)(const std::uint64_t&);
	static REL::Relocation<func_t> func{ ID::CollisionQuery::Cancel };
	static REL::Relocation<BSReadWriteLock*> lock{ ID::CollisionQuery::Lock };
	const BSAutoWriteLock locked{ lock.get() };
	return func(a_handle);
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

void RE::TES::LoadInterior(TESObjectCELL* a_cell, const NiPoint3& a_position, bool a_forceTransition)
{
	using func_t = void (*)(void*, TESObjectCELL*, const NiPoint3*, bool);
	static REL::Relocation<func_t> func{ ID::TES::LoadInterior };
	func(nullptr, a_cell, std::addressof(a_position), a_forceTransition);
}

void RE::TESObjectCELL::LoadSpaceReferences(bool a_updateLinkedRefs)
{
	using func_t = void (*)(TESObjectCELL*, bool, void*);
	static REL::Relocation<func_t> func{ ID::TESObjectCELL::LoadSpaceReferences };
	func(this, a_updateLinkedRefs, nullptr);
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
