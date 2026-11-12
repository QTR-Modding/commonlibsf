#pragma once

#include "RE/I/IPipelineStageInterface.h"

namespace RE
{
	struct IMovementPathManagerAgent : public IPipelineStageInterface
	{
	public:
		SF_RTTI(IMovementPathManagerAgent);
	};
	static_assert(sizeof(IMovementPathManagerAgent) == 0x8);
}
