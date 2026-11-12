#include "RE/S/SpaceshipSystems.h"
#include "RE/B/BSTObjectArena.h"
#include "RE/T/TESObjectREFR.h"

void RE::Spaceship::Systems::QueueSystemUpdates(const BSJobs2::JobContext& a_context)
{
	BSTObjectArena<std::uint32_t> forms;
	auto arena = std::addressof(forms);
	using manager_t = std::byte* (*)();
	static REL::Relocation<manager_t> manager{ ID::BSGalaxy::GetKnowledgeManager };
	using collect_t = void (*)(void*, BSTObjectArena<std::uint32_t>**);
	static REL::Relocation<collect_t> collect{ ID::Spaceship::Systems::CollectShips };
	collect(*reinterpret_cast<void**>(manager() + 0x8B0), std::addressof(arena));

	using eligible_t = bool (*)(const TESObjectREFR*);
	static REL::Relocation<eligible_t> eligible{ ID::Spaceship::Systems::IsEligible };
	static REL::Relocation<std::uint32_t*> invalidHandle{ ID::Spaceship::Systems::InvalidHandle };
	static REL::Relocation<void**> handleManager{ ID::Spaceship::Systems::HandleManager };
	using visit_t = void (*)(void*, std::uint32_t, void*);
	static REL::Relocation<visit_t> visit{ ID::Spaceship::Systems::VisitHandle };
	static REL::Relocation<std::uintptr_t> callbackVtable{ VTABLE::stl__local_function_detail__LocalFunctionImpl__lambda_2d0a379b148b2a2dd21b05b2bf8c5046__void_void_unsignedint_[0] };
	static REL::Relocation<std::uintptr_t> jobVtable{ VTABLE::BSJobs2__Detail__TNodeJob_void___void__void_[0] };
	static REL::Relocation<std::uintptr_t> invoke{ ID::BSJobs2::InvokePointerJob };
	static REL::Relocation<std::uintptr_t> update{ ID::Spaceship::Systems::UpdateShip };
	static REL::Relocation<std::uintptr_t> updateDisabled{ ID::Spaceship::Systems::UpdateDisabledShip };

	for (const auto formID : forms) {
		const TESForm* form = TESForm::LookupByID(formID);
		const TESObjectREFR* ship = form ? form->AsReference() : nullptr;
		if (!ship || !eligible(ship) || (ship->IsDeleted() && !ship->IsDisabled())) {
			continue;
		}

		auto handle = *invalidHandle;
		if (ship->nativeHandle != 0) {
			struct Capture
			{
				std::uint32_t* handle;
				const TESObjectREFR* reference;
			} capture{ std::addressof(handle), ship };
			struct Callback
			{
				std::uintptr_t vtable;
				Capture* capture;
			} callback{ callbackVtable.address(), std::addressof(capture) };
			visit(*handleManager, ship->nativeHandle, std::addressof(callback));
		}

		struct FunctionJob
		{
			std::uintptr_t vtable;
			std::uintptr_t invoke;
			BSJobs2::JobGroup* group;
			std::uintptr_t callback;
			std::uint64_t argument;
		};
		static_assert(sizeof(FunctionJob) == 0x28);
		const auto node = reinterpret_cast<FunctionJob*>(BSJobs2::AllocateJob(a_context.group, sizeof(FunctionJob), alignof(FunctionJob), a_context.prerequisite, a_context.dependent));
		node->vtable = jobVtable.address();
		node->invoke = invoke.address();
		node->group = a_context.group;
		node->callback = ship->IsDisabled() ? updateDisabled.address() : update.address();
		node->argument = handle;
	}
}
