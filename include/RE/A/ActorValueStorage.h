#pragma once

#include "RE/B/BSLock.h"
#include "RE/B/BSTArray.h"
#include "RE/B/BSTTuple.h"

namespace RE
{
	class ActorValueInfo;

	struct Modifiers
	{
	public:
		// members
		float modifiers[3];  // 0
	};
	static_assert(sizeof(Modifiers) == 0xC);

	class ActorValueStorage
	{
	public:
		// Raw overrides only; these do not resolve defaults or actor-value owner redirection.
		// GetBaseValue leaves a_value unchanged if there is no stored override.
		[[nodiscard]] bool  GetBaseValue(const ActorValueInfo& a_info, float& a_value) const;
		[[nodiscard]] float GetModifierSum(const ActorValueInfo& a_info) const;

		// members
		BSTArray<BSTTuple<ActorValueInfo*, float>>     baseValues;  // 00
		BSTArray<BSTTuple<ActorValueInfo*, Modifiers>> modifiers;   // 10
		BSReadWriteLock                              avLock;      // 20
	};
	static_assert(sizeof(ActorValueStorage) == 0x28);
	static_assert(sizeof(BSTTuple<ActorValueInfo*, float>) == 0x10);
	static_assert(sizeof(BSTTuple<ActorValueInfo*, Modifiers>) == 0x18);
	static_assert(offsetof(ActorValueStorage, baseValues) == 0x00);
	static_assert(offsetof(ActorValueStorage, modifiers) == 0x10);
	static_assert(offsetof(ActorValueStorage, avLock) == 0x20);
}
