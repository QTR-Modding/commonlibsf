#include "RE/S/SpaceshipMovement.h"

#include "RE/A/Actor.h"
#include "RE/B/BSLock.h"
#include "RE/T/TESObjectREFR.h"

#include <atomic>
#include <cmath>
#include <cstring>

namespace RE
{
	namespace
	{
		// Native PathingRequest starts with a vtable, type data and its own refcount.
		struct RequestPrefix
		{
			std::uintptr_t* vtable;
			std::byte       unk08[0x8];
			std::uint32_t   refCount;
			std::byte       unk14[0x3C];
			NiPoint3        facingPoint;
			float           angularTolerance;
			std::byte       unk60[0x2A];
			std::uint8_t    goalFlags;
		};
		static_assert(offsetof(RequestPrefix, refCount) == 0x10);
		static_assert(offsetof(RequestPrefix, facingPoint) == 0x50);
		static_assert(offsetof(RequestPrefix, angularTolerance) == 0x5C);
		static_assert(offsetof(RequestPrefix, goalFlags) == 0x8A);
		static_assert(sizeof(RequestPrefix) == 0x90);

		struct OpenSpaceRequestView
		{
			RequestPrefix prefix;
			std::byte     unk90[0x130];
			std::uint32_t entityID;
			TESFormID     shipID;
			TESFormID     exemptTargetID;
			std::uint8_t  specialFlight;
		};
		static_assert(offsetof(OpenSpaceRequestView, shipID) == 0x1C4);
		static_assert(offsetof(OpenSpaceRequestView, specialFlight) == 0x1CC);
		static_assert(sizeof(OpenSpaceRequestView) == 0x1D0);

		void ReleaseNative(void* a_object, std::size_t a_refCountOffset) noexcept
		{
			if (!a_object) {
				return;
			}
			auto* bytes = static_cast<std::byte*>(a_object);
			std::atomic_ref count{ *reinterpret_cast<std::uint32_t*>(bytes + a_refCountOffset) };
			if (count.fetch_sub(1) == 1) {
				using destructor_t = void* (*)(void*, std::uint32_t);
				const auto table = *reinterpret_cast<std::uintptr_t**>(a_object);
				reinterpret_cast<destructor_t>(table[0])(a_object, 1);
			}
		}

		struct NativeRequestOwner
		{
			PathingRequest* request{};
			NativeRequestOwner() = default;
			NativeRequestOwner(const NativeRequestOwner&) = delete;
			NativeRequestOwner& operator=(const NativeRequestOwner&) = delete;
			~NativeRequestOwner() { ReleaseNative(request, 0x10); }
		};
		static_assert(sizeof(NativeRequestOwner) == 0x8);

		struct ControllerView
		{
			std::byte unk00[0x48];
			void*     interfaces;
		};
		static_assert(offsetof(ControllerView, interfaces) == 0x48);

		// Physical component leases have a paired reference/weak count at owner-20h.
		// Actor controller views have a null owner. The native getter transfers this lease.
		struct ControllerLease
		{
			ControllerView* controller{};
			std::byte*      owner{};

			ControllerLease() = default;
			ControllerLease(const ControllerLease&) = delete;
			ControllerLease& operator=(const ControllerLease&) = delete;
			~ControllerLease()
			{
				if (owner) {
					std::atomic_ref counts{ *reinterpret_cast<std::uint64_t*>(owner - 0x20) };
					if (counts.fetch_sub(0x100000001ULL) == 0x100000001ULL) {
						using func_t = void (*)(void*);
						static REL::Relocation<func_t> func{ ID::Spaceship::Movement::ReleaseControllerLease };
						func(owner - 0x20);
					}
				}
			}
		};
		static_assert(sizeof(ControllerLease) == 0x10);

		struct RotationTarget
		{
			std::uint32_t mode;
			NiPoint3      point;
			float         angle;
			float         tolerance1;
			float         tolerance2;
		};
		static_assert(sizeof(RotationTarget) == 0x1C);

		struct FacingState
		{
			RotationTarget target;
			float          scale1;
			float          scale2;
		};
		static_assert(sizeof(FacingState) == 0x24);

		struct SetStateInterface
		{
			virtual ~SetStateInterface() = default;
			virtual void Set(const RotationTarget&, float, float) = 0;
		};
		struct SetRollInterface
		{
			virtual ~SetRollInterface() = default;
			virtual void Set(const NiPoint3&) = 0;
		};

		struct FlightAgentView
		{
			std::byte       unk00[0x7C];
			FacingState     facing;
			NiPoint3        up;
			std::byte       unkAC[0x24];
			BSReadWriteLock lock;
			std::byte       unkD8[0x8];
		};
		static_assert(offsetof(FlightAgentView, facing) == 0x7C);
		static_assert(offsetof(FlightAgentView, up) == 0xA0);
		static_assert(offsetof(FlightAgentView, lock) == 0xD0);
		static_assert(sizeof(FlightAgentView) == 0xE0);

		FlightAgentView* GetFlightAgent(TESObjectREFR* a_ship, ControllerLease& a_lease)
		{
			using getter_t = ControllerLease* (*)(ControllerLease*, TESObjectREFR*);
			static REL::Relocation<getter_t> getter{ ID::Spaceship::Movement::GetController };
			getter(std::addressof(a_lease), a_ship);
			if (!a_lease.controller || !a_lease.controller->interfaces) {
				return nullptr;
			}
			using lookup_t = SetStateInterface* (*)(void*, const void*);
			static REL::Relocation<lookup_t> lookup{ ID::Spaceship::Movement::LookupInterface };
			static REL::Relocation<const void*> key{ ID::Spaceship::Movement::StateInterfaceKey };
			auto* state = lookup(a_lease.controller->interfaces, key.get());
			if (!state) {
				return nullptr;
			}
			// Native vtable ID 479100 is the concrete flight agent's SetState base at 0x40.
			// Its complete-object locator and setter bodies prove the view below.
			static REL::Relocation<std::uintptr_t> table{ VTABLE::MovementAgentFlightController[8] };
			if (*reinterpret_cast<const std::uintptr_t*>(state) != table.address()) {
				return nullptr;
			}
			return reinterpret_cast<FlightAgentView*>(reinterpret_cast<std::byte*>(state) - 0x40);
		}

		SetStateInterface* StateInterface(FlightAgentView* a_agent)
		{
			return reinterpret_cast<SetStateInterface*>(reinterpret_cast<std::byte*>(a_agent) + 0x40);
		}
		SetRollInterface* RollInterface(FlightAgentView* a_agent)
		{
			return reinterpret_cast<SetRollInterface*>(reinterpret_cast<std::byte*>(a_agent) + 0x48);
		}
	}

	BSTEventSource<BSPathEvent>* BSPathEvent::GetEventSource()
	{
		static REL::Relocation<std::byte**> manager{ ID::Spaceship::Movement::EventManager };
		return *manager ? reinterpret_cast<BSTEventSource<BSPathEvent>*>(*manager + 0x10) : nullptr;
	}

	namespace Spaceship::Movement
	{
		PointRequest::~PointRequest() { Release(); }
		PointRequest::PointRequest(PointRequest&& a_rhs) noexcept :
			_ship(std::move(a_rhs._ship)),
			_request(std::exchange(a_rhs._request, nullptr)),
			_listenerID(std::exchange(a_rhs._listenerID, -1))
		{}
		PointRequest& PointRequest::operator=(PointRequest&& a_rhs) noexcept
		{
			if (this != std::addressof(a_rhs)) {
				Release();
				_ship = std::move(a_rhs._ship);
				_request = std::exchange(a_rhs._request, nullptr);
				_listenerID = std::exchange(a_rhs._listenerID, -1);
			}
			return *this;
		}
		void PointRequest::Release() noexcept
		{
			ReleaseNative(std::exchange(_request, nullptr), 0x10);
			_ship.reset();
			_listenerID = -1;
		}
		bool PointRequest::IsCurrent() const
		{
			if (!_request) {
				return false;
			}
			NativeRequestOwner current;
			using func_t = bool (*)(TESObjectREFR*, NativeRequestOwner*);
			static REL::Relocation<func_t> func{ ID::Spaceship::Movement::QueryCurrent };
			return func(_ship.get(), std::addressof(current)) && current.request == _request;
		}
		bool PointRequest::Cancel() const
		{
			if (!IsCurrent()) {
				return false;
			}
			using func_t = void (*)(TESObjectREFR*);
			static REL::Relocation<func_t> func{ ID::Spaceship::Movement::CancelRequest };
			func(_ship.get());
			return true;
		}

		NiPointer<Actor> ResolveActor(TESObjectREFR* a_ship)
		{
			NiPointer<Actor> actor;
			using func_t = NiPointer<Actor>* (*)(NiPointer<Actor>*, TESFormID);
			static REL::Relocation<func_t> func{ ID::Spaceship::Movement::ResolveActor };
			func(std::addressof(actor), a_ship->GetFormID());
			return actor;
		}

		bool RequestPoint(TESObjectREFR* a_ship, const NiPoint3& a_position, TESObjectCELL* a_cell,
			TESWorldSpace* a_worldspace, float a_radius, PointRequest& a_out, const NiPoint3* a_facingPoint)
		{
			if (!std::isfinite(a_radius) || a_radius <= 0.0F || !BSPathEvent::GetEventSource()) {
				return false;
			}
			auto actor = ResolveActor(a_ship);
			if (!actor) {
				return false;
			}
			if (actor->IsInCombat()) {
				return false;
			}
			PointRequest pending;
			pending._ship.reset(a_ship);
			using create_t = void (*)(Actor*, TESObjectREFR*, PathingRequest**);
			static REL::Relocation<create_t> create{ ID::Spaceship::Movement::CreateRequest };
			create(actor.get(), nullptr, std::addressof(pending._request));
			if (!pending._request) {
				return false;
			}
			const auto& openSpace = *reinterpret_cast<const OpenSpaceRequestView*>(pending._request);
			static REL::Relocation<std::uintptr_t> requestTable{ VTABLE::PathingRequestOpenSpace[0] };
			if (reinterpret_cast<std::uintptr_t>(openSpace.prefix.vtable) != requestTable.address() ||
				openSpace.shipID != a_ship->GetFormID()) {
				return false;
			}
			// A null native waypoint smart pointer, passed by address like the stock caller.
			void* waypoints{};
			using configure_t = void (*)(Actor*, PathingRequest**, const NiPoint3*, TESObjectCELL*, TESWorldSpace*, float, std::uint8_t, void**, bool);
			static REL::Relocation<configure_t> configure{ ID::Spaceship::Movement::ConfigurePoint };
			configure(actor.get(), std::addressof(pending._request), std::addressof(a_position), a_cell,
				a_worldspace, a_radius, 2, std::addressof(waypoints), false);
			if (a_facingPoint) {
				auto& request = *reinterpret_cast<RequestPrefix*>(pending._request);
				request.facingPoint = *a_facingPoint;
				request.goalFlags = static_cast<std::uint8_t>((request.goalFlags & ~0x14U) | 0x08U);
			}
			using submit_t = bool (*)(Actor*, PathingRequest**);
			static REL::Relocation<submit_t> submit{ ID::Spaceship::Movement::Submit };
			if (!submit(actor.get(), std::addressof(pending._request))) {
				return false;
			}
			using register_t = std::int32_t (*)(void*, PathingRequest*);
			static REL::Relocation<void**> manager{ ID::Spaceship::Movement::EventManager };
			static REL::Relocation<register_t> registerRequest{ ID::Spaceship::Movement::RegisterRequest };
			pending._listenerID = registerRequest(*manager, pending._request);
			if (pending._listenerID == -1) {
				(void)pending.Cancel();
				return false;
			}
			a_out = std::move(pending);
			return true;
		}

		FlightOrientation::~FlightOrientation() { Release(); }
		FlightOrientation::FlightOrientation(FlightOrientation&& a_rhs) noexcept :
			_agent(std::exchange(a_rhs._agent, nullptr)), _facing(a_rhs._facing), _up(a_rhs._up)
		{}
		FlightOrientation& FlightOrientation::operator=(FlightOrientation&& a_rhs) noexcept
		{
			if (this != std::addressof(a_rhs)) {
				Release();
				_agent = std::exchange(a_rhs._agent, nullptr);
				_facing = a_rhs._facing;
				_up = a_rhs._up;
			}
			return *this;
		}
		void FlightOrientation::Release() noexcept { ReleaseNative(std::exchange(_agent, nullptr), 0x8); }

		bool CaptureOrientation(TESObjectREFR* a_ship, FlightOrientation& a_out)
		{
			ControllerLease lease;
			auto* agent = GetFlightAgent(a_ship, lease);
			if (!agent) {
				return false;
			}
			FlightOrientation snapshot;
			std::atomic_ref count{ *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::byte*>(agent) + 0x8) };
			count.fetch_add(1);
			snapshot._agent = agent;
			{
				BSAutoReadLock lock{ agent->lock };
				std::memcpy(snapshot._facing.data(), std::addressof(agent->facing), sizeof(FacingState));
				snapshot._up = agent->up;
			}
			a_out = std::move(snapshot);
			return true;
		}

		bool SetUpVector(TESObjectREFR* a_ship, const NiPoint3& a_up)
		{
			ControllerLease lease;
			auto* agent = GetFlightAgent(a_ship, lease);
			if (!agent) {
				return false;
			}
			RollInterface(agent)->Set(a_up);
			return true;
		}

		bool RestoreOrientation(TESObjectREFR* a_ship, const FlightOrientation& a_snapshot)
		{
			if (!a_snapshot) {
				return false;
			}
			ControllerLease lease;
			auto* agent = GetFlightAgent(a_ship, lease);
			if (agent != a_snapshot._agent) {
				return false;
			}
			FacingState state;
			std::memcpy(std::addressof(state), a_snapshot._facing.data(), sizeof(state));
			StateInterface(agent)->Set(state.target, state.scale1, state.scale2);
			RollInterface(agent)->Set(a_snapshot._up);
			return true;
		}
	}
}
