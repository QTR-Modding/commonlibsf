#pragma once

#include "RE/A/ActorValueStorage.h"
#include "RE/B/BSExtraData.h"

namespace RE
{
	// Actor values on non-Actor references, including physical spaceship references.
	class ExtraActorValueStorage : public BSExtraData
	{
	public:
		SF_RTTI_VTABLE(ExtraActorValueStorage);
		SF_EXTRADATATYPE(ActorValueStorage);

		~ExtraActorValueStorage() override;  // 00

		// members
		ActorValueStorage avStorage;  // 18
	};
	static_assert(sizeof(ExtraActorValueStorage) == 0x40);
	static_assert(offsetof(ExtraActorValueStorage, avStorage) == 0x18);
}
