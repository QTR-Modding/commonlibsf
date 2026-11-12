#include "RE/V/ViewSpace.h"
#include "ViewSpaceOriginScope.h"

#include "RE/P/PlayerCamera.h"
#include "RE/T/TESObjectREFR.h"

RE::ViewSpaceOriginScope::ViewSpaceOriginScope(void* a_view)
{
	using func_t = void** (*)(void* const*, void**);
	static REL::Relocation<func_t> acquire{ ID::ViewSpace::AcquireWrite };
	acquire(std::addressof(a_view), std::addressof(_view));
}

RE::ViewSpaceOriginScope::~ViewSpaceOriginScope()
{
	using func_t = void (*)(void**);
	static REL::Relocation<func_t> release{ ID::ViewSpace::ReleaseWrite };
	release(std::addressof(_view));
}

std::byte* RE::ViewSpaceOriginScope::GetData() const
{
	static REL::Relocation<const std::uint16_t*> component{ ID::ViewSpace::HighDetailInfo };
	using func_t = bool (*)(void*, Cursor*, const std::uint64_t*);
	static REL::Relocation<func_t> lookup{ ID::ViewSpace::LookupComponent };
	const std::uint64_t key = static_cast<std::uint64_t>(*component) << 48;
	Cursor cursor;
	if (!lookup(_view, std::addressof(cursor), std::addressof(key))) {
		return nullptr;
	}
	std::byte* page = reinterpret_cast<std::byte*>(cursor.pages[cursor.depth - 1]);
	const std::uint32_t dataOffset = *reinterpret_cast<const std::uint32_t*>(page + 4);
	const std::uint16_t entryOffset = *reinterpret_cast<const std::uint16_t*>(
		page + dataOffset + (cursor.indices[cursor.depth - 1] + 1) * 2);
	return page + dataOffset + entryOffset + 0x10;
}

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

void RE::ViewSpace::SelectReference(std::uint32_t a_systemID, TESObjectREFR* a_reference)
{
	using func_t = void (*)(std::uint32_t, TESFormID);
	static REL::Relocation<func_t> select{ ID::ViewSpace::SelectReference };
	select(a_systemID, a_reference->GetSpaceBodyID());
}

void RE::ViewSpace::SetFloatingOrigin(const std::array<double, 4>& a_origin)
{
	using func_t = void (*)(const double*, bool, bool);
	static REL::Relocation<func_t> set{ ID::ViewSpace::SetFloatingOrigin };
	set(a_origin.data(), false, true);
}

bool RE::ViewSpace::CaptureOrigin(Origin& a_out) const
{
	if (!_space) {
		return false;
	}
	static REL::Relocation<const std::array<double, 4>*> floatingOrigin{ ID::Spaceship::Docking::FloatingOrigin };
	Origin origin{};
	origin.floatingOrigin = *floatingOrigin.get();
	ViewSpaceOriginScope scope{ _space };
	if (const std::byte* data = scope.GetData()) {
		origin.anchor = *reinterpret_cast<const std::array<double, 4>*>(data + 0x30);
		origin.fittedPosition = *reinterpret_cast<const std::array<double, 4>*>(data + 0x50);
		origin.velocity = *reinterpret_cast<const NiPoint3*>(data + 0x70);
		origin.hasHighDetailInfo = true;
	}
	a_out = origin;
	return true;
}

bool RE::ViewSpace::RestoreOrigin(const Origin& a_origin) const
{
	if (!_space) {
		return false;
	}
	if (a_origin.hasHighDetailInfo) {
		ViewSpaceOriginScope scope{ _space };
		std::byte* data = scope.GetData();
		if (!data) {
			return false;
		}
		*reinterpret_cast<std::uint32_t*>(static_cast<std::byte*>(_space) + 0x10) = 0;
		*reinterpret_cast<std::array<double, 4>*>(data + 0x30) = a_origin.anchor;
		*reinterpret_cast<std::array<double, 4>*>(data + 0x50) = a_origin.fittedPosition;
		*reinterpret_cast<NiPoint3*>(data + 0x70) = a_origin.velocity;
	}
	SetFloatingOrigin(a_origin.floatingOrigin);
	return true;
}
