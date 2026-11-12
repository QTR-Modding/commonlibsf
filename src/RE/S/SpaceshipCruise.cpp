#include "RE/S/SpaceshipCruise.h"

namespace RE::Spaceship::Cruise
{
	bool CanStart()
	{
		using func_t = decltype(&CanStart);
		static REL::Relocation<func_t> func{ ID::Spaceship::Cruise::CanStart };
		return func();
	}

	bool IsActive()
	{
		using func_t = decltype(&IsActive);
		static REL::Relocation<func_t> func{ ID::Spaceship::Cruise::IsActive };
		return func();
	}

	std::uint32_t GetCourse()
	{
		static REL::Relocation<const std::uint32_t*> course{ ID::Spaceship::Cruise::Course };
		return *course;
	}

	void ToggleCourse(std::uint32_t a_destinationID)
	{
		using func_t = decltype(&ToggleCourse);
		static REL::Relocation<func_t> func{ ID::Spaceship::Cruise::ToggleCourse };
		func(a_destinationID);
	}

	void Start(bool a_restoreState)
	{
		using func_t = decltype(&Start);
		static REL::Relocation<func_t> func{ ID::Spaceship::Cruise::Start };
		func(a_restoreState);
	}

	void RequestExit()
	{
		using func_t = decltype(&RequestExit);
		static REL::Relocation<func_t> func{ ID::Spaceship::Cruise::RequestExit };
		func();
	}

	void Stop()
	{
		using func_t = decltype(&Stop);
		static REL::Relocation<func_t> func{ ID::Spaceship::Cruise::Stop };
		func();
	}
}
