#pragma once

#include "RE/B/BSTEvent.h"
#include "RE/N/NiPoint.h"
#include "RE/N/NiSmartPointer.h"

#include <array>

namespace RE
{
	class Actor;
	class PathingRequest;
	class TESObjectCELL;
	class TESObjectREFR;
	class TESWorldSpace;

	struct BSPathEvent
	{
		// Result 0 is completion; 1 is failure/replacement, 2 replaces StopMoving, 3 interrupts.
		std::int32_t  listenerID;
		std::uint32_t result;

		[[nodiscard]] static BSTEventSource<BSPathEvent>* GetEventSource();
	};
	static_assert(sizeof(BSPathEvent) == 0x8);

	namespace Spaceship::Movement
	{
		// These operations run on the game thread with a valid physical ship.
		// A handle owns the native request and ship. Releasing it does not cancel flight.
		class PointRequest
		{
		public:
			PointRequest() noexcept = default;
			~PointRequest();
			PointRequest(PointRequest&& a_rhs) noexcept;
			PointRequest& operator=(PointRequest&& a_rhs) noexcept;
			PointRequest(const PointRequest&) = delete;
			PointRequest& operator=(const PointRequest&) = delete;

			[[nodiscard]] explicit operator bool() const noexcept { return _request != nullptr; }
			[[nodiscard]] std::int32_t ListenerID() const noexcept { return _listenerID; }
			[[nodiscard]] bool IsCurrent() const;
			// Clears only this request while it is current. This is not physical braking.
			// Native cancellation delivers its event synchronously before clearing the path.
			[[nodiscard]] bool Cancel() const;

		private:
			friend bool RequestPoint(TESObjectREFR*, const NiPoint3&, TESObjectCELL*, TESWorldSpace*, float, PointRequest&, const NiPoint3*, TESObjectREFR*);
			void Release() noexcept;

			NiPointer<TESObjectREFR> _ship;
			PathingRequest*         _request{};
			std::int32_t            _listenerID{ -1 };
		};

		// Exact ship-AI lookup used by the stock SpaceshipReference.PathToReference binding.
		// The returned pointer owns its reference; this does not assign a player pilot role.
		[[nodiscard]] NiPointer<Actor> ResolveActor(TESObjectREFR* a_ship);

		// Copies an origin-resolved common-space XYZ into native frame-owned path data.
		// CELL/worldspace describe that coordinate system; inputs are borrowed during the call.
		// Native movement parameters and obstacle avoidance remain in charge. Radius must be positive.
		// Optional facingPoint uses the same origin-resolved coordinate system, not a direction.
		// It preserves the native angular tolerance and gates the final node's forward angle.
		// Optional avoidanceTarget must be a valid physical ship; its steering constraint is exempt.
		// Other obstacles and physical collisions remain enabled. Null preserves ordinary avoidance.
		// Completion does not certify roll alignment or zero physical velocity.
		// Register an event sink first and match ListenerID; submission replaces previous movement.
		// On failure a_out is unchanged. Native submission may already have replaced an old request.
		[[nodiscard]] bool RequestPoint(TESObjectREFR* a_ship, const NiPoint3& a_position, TESObjectCELL* a_cell,
			TESWorldSpace* a_worldspace, float a_radius, PointRequest& a_out, const NiPoint3* a_facingPoint = nullptr,
			TESObjectREFR* a_avoidanceTarget = nullptr);

		// Requests a normal stop through the ship AI actor, respecting native admission.
		// This can replace current movement and reports neither acceptance nor physical rest.
		void RequestStop(TESObjectREFR* a_ship);

		// Opaque native facing target, both scales and up vector, with strong agent ownership.
		// Keep this snapshot for restoration; it is not an orientation-completion certificate.
		class FlightOrientation
		{
		public:
			FlightOrientation() noexcept = default;
			~FlightOrientation();
			FlightOrientation(FlightOrientation&& a_rhs) noexcept;
			FlightOrientation& operator=(FlightOrientation&& a_rhs) noexcept;
			FlightOrientation(const FlightOrientation&) = delete;
			FlightOrientation& operator=(const FlightOrientation&) = delete;
			[[nodiscard]] explicit operator bool() const noexcept { return _agent != nullptr; }

		private:
			friend bool CaptureOrientation(TESObjectREFR*, FlightOrientation&);
			friend bool RestoreOrientation(TESObjectREFR*, const FlightOrientation&);
			void Release() noexcept;

			void*                       _agent{};
			std::array<std::byte, 0x24> _facing{};
			NiPoint3                    _up;
		};

		// Only the verified native MovementAgentFlightController is supported.
		[[nodiscard]] bool CaptureOrientation(TESObjectREFR* a_ship, FlightOrientation& a_out);
		// Up is a world-space vector; the current facing state is preserved.
		[[nodiscard]] bool SetUpVector(TESObjectREFR* a_ship, const NiPoint3& a_up);
		// Fails without writing if the ship now has a different native flight agent.
		[[nodiscard]] bool RestoreOrientation(TESObjectREFR* a_ship, const FlightOrientation& a_snapshot);
	}
}
