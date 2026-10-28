#pragma once

#include "RE/B/BSInputEventReceiver.h"

namespace RE::PlayerControlsManager
{
	// PlayerControls::Manager remains opaque. This BSInputEventReceiver address
	// point and callback ABI are verified for Starfield 1.16.244.
	inline constexpr REL::ID BSINPUTEVENTRECEIVER_VTABLE =
		RE::VTABLE::PlayerControls__Manager[8];

	inline constexpr std::size_t kPerformInputProcessingVFunc = 1;
	using PerformInputProcessing_t = void(BSInputEventReceiver*, const InputEvent*);

	struct FlightThrottle
	{
		float target;
		float current;
	};

	// Call on the game thread after flight controls have acquired a valid ship.
	// Failure leaves a_out unchanged.
	[[nodiscard]] bool GetFlightThrottle(FlightThrottle& a_out);

	// Requests normal throttle in [0, 1]. Changes only the target; the native flight
	// update applies it. Input modes may replace it on a later update. This does not
	// set physical velocity, engage boosters or change cruise settings.
	// Returns false for an invalid value or when flight controls have no valid ship.
	[[nodiscard]] bool SetFlightTargetThrottle(float a_throttle);
}
