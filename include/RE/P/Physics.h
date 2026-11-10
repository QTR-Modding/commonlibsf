#pragma once

#include "RE/B/BSTObjectArena.h"

namespace RE
{
	class bhkWorld;
}

namespace RE::Physics
{
	using WorldCollection = BSTObjectArena<bhkWorld*>;
	static_assert(sizeof(WorldCollection) == 0x40);

	// Includes the existing space world once in an engine-owned frame collection.
	// Call on the collecting thread; the collection and worlds remain borrowed.
	void IncludeSpaceWorld(WorldCollection& a_worlds);
}
