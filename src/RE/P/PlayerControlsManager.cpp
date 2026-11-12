#include "RE/P/PlayerControlsManager.h"

namespace RE::PlayerControlsManager
{
	namespace
	{
		struct FlightMovementHandler
		{
			std::byte    unk00[0x68];
			float        targetThrottle;  // 68
			float        currentThrottle;  // 6C
			std::byte    unk70[0xC];
			std::uint8_t hasValidShip;     // 7C
		};
		static_assert(offsetof(FlightMovementHandler, targetThrottle) == 0x68);
		static_assert(offsetof(FlightMovementHandler, currentThrottle) == 0x6C);
		static_assert(offsetof(FlightMovementHandler, hasValidShip) == 0x7C);

		struct Manager
		{
			std::byte              unk00[0x208];
			FlightMovementHandler* flightMovementHandler;  // 208
		};
		static_assert(offsetof(Manager, flightMovementHandler) == 0x208);

		FlightMovementHandler* GetFlightHandler()
		{
			static REL::Relocation<Manager**> singleton{ ID::PlayerControlsManager::Singleton };
			auto* manager = *singleton;
			auto* handler = manager ? manager->flightMovementHandler : nullptr;
			return handler && handler->hasValidShip ? handler : nullptr;
		}
	}

	bool GetFlightThrottle(FlightThrottle& a_out)
	{
		if (auto* handler = GetFlightHandler()) {
			a_out = { handler->targetThrottle, handler->currentThrottle };
			return true;
		}
		return false;
	}

	bool SetFlightTargetThrottle(float a_throttle)
	{
		if (!(a_throttle >= 0.0F && a_throttle <= 1.0F)) {
			return false;
		}
		if (auto* handler = GetFlightHandler()) {
			handler->targetThrottle = a_throttle;
			return true;
		}
		return false;
	}
}
