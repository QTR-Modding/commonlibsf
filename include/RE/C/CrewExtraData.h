#pragma once

#include "RE/B/BSCoreTypes.h"
#include "RE/B/BSExtraData.h"
#include "RE/B/BSPointerHandle.h"
#include "RE/B/BSTArray.h"

namespace RE
{
	class TESObjectREFR;

	class CrewExtraData : public BSExtraData
	{
	public:
		SF_RTTI_VTABLE(CrewExtraData);
		SF_EXTRADATATYPE(Crew);

		~CrewExtraData() override;  // 00

		// members
		BSTArray<BSPointerHandle<TESObjectREFR>> members;  // 18
		TESFormID                                ownerID;  // 28
		std::uint32_t                            pad2C;    // 2C
	};
	static_assert(offsetof(CrewExtraData, members) == 0x18);
	static_assert(offsetof(CrewExtraData, ownerID) == 0x28);
	static_assert(sizeof(CrewExtraData) == 0x30);
}
