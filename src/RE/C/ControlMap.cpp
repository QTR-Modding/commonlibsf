#include "RE/C/ControlMap.h"

namespace RE
{
	ControlMap* ControlMap::GetSingleton()
	{
		static REL::Relocation<ControlMap**> singleton{ ID::ControlMap::Singleton };
		return *singleton;
	}

	void ControlMap::PushInputContext(Context a_context, const BSFixedString& a_owner)
	{
		using func_t = decltype(&ControlMap::PushInputContext);
		static REL::Relocation<func_t> func{ ID::ControlMap::PushInputContext };
		func(this, a_context, a_owner);
	}

	bool ControlMap::PopInputContext(Context a_context, const BSFixedString& a_owner)
	{
		using func_t = decltype(&ControlMap::PopInputContext);
		static REL::Relocation<func_t> func{ ID::ControlMap::PopInputContext };
		return func(this, a_context, a_owner);
	}
}
