#pragma once

#include "RE/I/IPipelineStageInterface.h"

namespace RE
{
	struct IMovementPlannerAgent : public IPipelineStageInterface
	{
	public:
		SF_RTTI(IMovementPlannerAgent);
	};
	static_assert(sizeof(IMovementPlannerAgent) == 0x8);
}
