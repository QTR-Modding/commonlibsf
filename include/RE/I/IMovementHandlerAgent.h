#pragma once

#include "RE/I/IPipelineStageInterface.h"

namespace RE
{
	struct IMovementHandlerAgent : public IPipelineStageInterface
	{
	public:
		SF_RTTI(IMovementHandlerAgent);
	};
	static_assert(sizeof(IMovementHandlerAgent) == 0x8);
}
