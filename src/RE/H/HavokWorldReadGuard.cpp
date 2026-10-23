#include "RE/H/HavokWorldReadGuard.h"

#include "RE/B/BSLock.h"

namespace RE::Havok
{
	namespace
	{
		// Verified lock location only; the complete world hierarchy remains opaque.
		struct WorldLockLayout
		{
			std::byte       unk00[0xB20];
			BSReadWriteLock lock;
		};
		static_assert(offsetof(WorldLockLayout, lock) == 0xB20);
	}

	WorldReadGuard::WorldReadGuard(hknpBSWorld* a_world)
	{
		using func_t = WorldReadGuard* (*)(WorldReadGuard*, hknpBSWorld*, std::uint32_t);
		static REL::Relocation<func_t> func{ ID::Havok::WorldReadGuard::Acquire };
		func(this, a_world, 0);
	}

	WorldReadGuard::~WorldReadGuard()
	{
		// Native acquisition can replace both fields using the thread context.
		if (_world && _mode <= 1) {
			reinterpret_cast<WorldLockLayout*>(_world)->lock.UnlockRead();
		}
	}
}
