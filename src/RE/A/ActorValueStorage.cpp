#include "RE/A/ActorValueStorage.h"

namespace RE
{
	bool ActorValueStorage::GetBaseValue(const ActorValueInfo& a_info, float& a_value) const
	{
		using func_t = decltype(&ActorValueStorage::GetBaseValue);
		static REL::Relocation<func_t> func{ ID::ActorValueStorage::GetBaseValue };
		return func(this, a_info, a_value);
	}

	float ActorValueStorage::GetModifierSum(const ActorValueInfo& a_info) const
	{
		using func_t = decltype(&ActorValueStorage::GetModifierSum);
		static REL::Relocation<func_t> func{ ID::ActorValueStorage::GetModifierSum };
		return func(this, a_info);
	}
}
