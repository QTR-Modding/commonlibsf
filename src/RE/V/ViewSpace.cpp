#include "RE/V/ViewSpace.h"

#include "RE/P/PlayerCamera.h"

RE::ViewSpace::ViewSpace() noexcept = default;

RE::ViewSpace::~ViewSpace()
{
	Release();
}

RE::ViewSpace::ViewSpace(ViewSpace&& a_rhs) noexcept :
	_space(std::exchange(a_rhs._space, nullptr))
{}

RE::ViewSpace& RE::ViewSpace::operator=(ViewSpace&& a_rhs) noexcept
{
	if (this != std::addressof(a_rhs)) {
		Release();
		_space = std::exchange(a_rhs._space, nullptr);
	}
	return *this;
}

RE::ViewSpace::operator bool() const noexcept
{
	return _space != nullptr;
}

RE::ViewSpace RE::ViewSpace::Capture()
{
	static REL::Relocation<void**> manager{ ID::ViewSpace::Manager };
	ViewSpace snapshot;
	snapshot._space = *reinterpret_cast<void**>(static_cast<std::byte*>(*manager) + 0x58);
	if (snapshot._space) {
		std::atomic_ref count{ *reinterpret_cast<std::uint64_t*>(static_cast<std::byte*>(snapshot._space) + 0x18) };
		count.fetch_add(1);
	}
	return snapshot;
}

void RE::ViewSpace::SelectSystem(std::uint32_t a_systemID)
{
	static REL::Relocation<void**> manager{ ID::ViewSpace::Manager };
	using func_t = void (*)(void*, std::uint32_t);
	static REL::Relocation<func_t> func{ ID::ViewSpace::SelectSystem };
	func(*manager, a_systemID);
	ViewSpace current = Capture();
	PlayerCamera* camera = PlayerCamera::GetSingleton();
	if (camera->viewSpace != current._space) {
		std::swap(camera->viewSpace, current._space);
	}
}

void RE::ViewSpace::Select() const
{
	static REL::Relocation<void**> manager{ ID::ViewSpace::Manager };
	using func_t = void (*)(void*, void* const*);
	static REL::Relocation<func_t> func{ ID::ViewSpace::Select };
	func(*manager, std::addressof(_space));
}

void RE::ViewSpace::Release() noexcept
{
	void* previous = std::exchange(_space, nullptr);
	if (previous) {
		using func_t = void (*)(void**);
		static REL::Relocation<func_t> func{ ID::ViewSpace::Release };
		func(std::addressof(previous));
	}
}
