#pragma once

#include "RE/B/BSInputEventReceiver.h"

namespace RE::BSInputDeviceManagerInput
{
	// BSInputDeviceManager remains opaque. This BSInputEventReceiver address
	// point and callback ABI are verified for Starfield 1.16.244.
	inline constexpr REL::ID BSINPUTEVENTRECEIVER_VTABLE =
		RE::VTABLE::BSInputDeviceManager[1];

	inline constexpr std::size_t kPerformInputProcessingVFunc = 1;
	using PerformInputProcessing_t = void(BSInputEventReceiver*, const InputEvent*);
}
