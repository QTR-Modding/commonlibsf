#include "RE/B/BGSGenericBaseForm.h"

namespace RE
{
	bool BGSGenericBaseForm::IsSpaceshipBase() const
	{
		using func_t = decltype(&BGSGenericBaseForm::IsSpaceshipBase);
		static REL::Relocation<func_t> func{ ID::BGSGenericBaseForm::IsSpaceshipBase };
		return func(this);
	}
}
