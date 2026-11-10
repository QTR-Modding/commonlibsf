#include "RE/S/SpaceshipInteriorLoad.h"
#include "SpaceshipComponentLease.h"

#include "RE/T/TESObjectREFR.h"
#include "RE/T/TESObjectCELL.h"

#include <atomic>
#include <utility>

RE::Spaceship::InteriorLoad::InteriorLoad() noexcept = default;

RE::Spaceship::InteriorLoad::~InteriorLoad()
{
	if (!Reset()) {
		Abandon();
	}
}

RE::Spaceship::InteriorLoad::InteriorLoad(InteriorLoad&& a_rhs) noexcept :
	_ship(std::move(a_rhs._ship)),
	_resource(std::exchange(a_rhs._resource, nullptr)),
	_ownsLoad(std::exchange(a_rhs._ownsLoad, false))
{}

RE::Spaceship::InteriorLoad& RE::Spaceship::InteriorLoad::operator=(InteriorLoad&& a_rhs) noexcept
{
	if (this != std::addressof(a_rhs)) {
		if (!Reset()) {
			Abandon();
		}
		_ship = std::move(a_rhs._ship);
		_resource = std::exchange(a_rhs._resource, nullptr);
		_ownsLoad = std::exchange(a_rhs._ownsLoad, false);
	}
	return *this;
}

RE::Spaceship::InteriorLoad::operator bool() const noexcept
{
	return _resource != nullptr;
}

RE::Spaceship::InteriorLoad::Status RE::Spaceship::InteriorLoad::GetStatus() const
{
	if (!_resource) {
		return Status::Replaced;
	}
	void* current = CaptureResource(_ship.get());
	const bool same = current == _resource;
	ReleaseResource(current);
	return same ? GetResourceStatus() : Status::Replaced;
}

RE::Spaceship::InteriorLoad::Status RE::Spaceship::InteriorLoad::GetResourceStatus() const
{
	static REL::Relocation<std::byte**> manager{ ID::Spaceship::InteriorLoad::IOManager };
	BSAutoLock guard{ reinterpret_cast<BSSpinLock*>(*manager + 0x280) };
	const std::byte* resource = static_cast<const std::byte*>(_resource);
	const std::uint32_t state = *reinterpret_cast<const std::uint32_t*>(resource + 0x14) & 0x70000000U;
	if (state == 0x10000000U || state == 0x20000000U) {
		return Status::Pending;
	}
	if (state != 0x30000000U && state != 0x40000000U) {
		return Status::Failed;
	}
	const std::uint64_t lifecycle = *reinterpret_cast<const std::uint64_t*>(resource + 0x28);
	// The native queue lock excludes the transient pre-attach 4 through its callback.
	// A 4 observed here is the completed detach state.
	if (lifecycle == 4) {
		return Status::Failed;
	}
	if (lifecycle != 6) {
		return Status::Pending;
	}
	TESObjectCELL* cell = *reinterpret_cast<TESObjectCELL* const*>(resource + 0x30);
	return cell && cell->IsAttached() ? Status::Attached : Status::Failed;
}

RE::Spaceship::InteriorLoad RE::Spaceship::InteriorLoad::Request(TESObjectREFR* a_ship)
{
	InteriorLoad request;
	if (!a_ship) {
		return request;
	}
	request._ship.reset(a_ship);
	void* previous = CaptureResource(a_ship);
	const std::uint32_t state = previous ?
		(*reinterpret_cast<const std::uint32_t*>(static_cast<const std::byte*>(previous) + 0x14) & 0x70000000U) : 0;
	const bool existing = state >= 0x10000000U && state <= 0x40000000U;
	using func_t = void (*)(TESObjectREFR*);
	static REL::Relocation<func_t> load{ ID::Spaceship::InteriorLoad::Request };
	load(a_ship);
	request._resource = CaptureResource(a_ship);
	request._ownsLoad = !existing && request._resource;
	ReleaseResource(previous);
	return request;
}

bool RE::Spaceship::InteriorLoad::Reset()
{
	if (_ownsLoad && GetResourceStatus() == Status::Pending) {
		return false;
	}
	void* resource = std::exchange(_resource, nullptr);
	const bool owned = std::exchange(_ownsLoad, false);
	NiPointer<TESObjectREFR> ship = std::move(_ship);
	if (resource && owned) {
		void* current = CaptureResource(ship.get());
		if (current == resource) {
			using func_t = void (*)(TESObjectREFR*, std::uint8_t);
			static REL::Relocation<func_t> unload{ ID::Spaceship::InteriorLoad::Unload };
			unload(ship.get(), 1);
		}
		ReleaseResource(current);
	}
	ReleaseResource(resource);
	return true;
}

void RE::Spaceship::InteriorLoad::Abandon() noexcept
{
	_ownsLoad = false;
	ReleaseResource(std::exchange(_resource, nullptr));
	_ship.reset();
}

void* RE::Spaceship::InteriorLoad::CaptureResource(TESObjectREFR* a_ship)
{
	detail::ComponentLease<std::byte> cells;
	using func_t = decltype(cells)* (*)(decltype(cells)*, TESFormID);
	static REL::Relocation<func_t> get{ ID::TESObjectREFR::GetSpaceshipLinkedCells };
	get(std::addressof(cells), a_ship->GetFormID());
	void* resource = cells.value ? *reinterpret_cast<void**>(cells.value + 0x18) : nullptr;
	if (resource) {
		std::atomic_ref count{ *reinterpret_cast<std::uint32_t*>(static_cast<std::byte*>(resource) + 0x14) };
		count.fetch_add(1);
	}
	return resource;
}

void RE::Spaceship::InteriorLoad::ReleaseResource(void* a_resource) noexcept
{
	if (a_resource) {
		using func_t = void (*)(void*);
		static REL::Relocation<func_t> release{ ID::Spaceship::InteriorLoad::ReleaseResource };
		release(a_resource);
	}
}
