#pragma once

namespace RE
{
	class hknpBSWorld;

	namespace Havok
	{
		// Keep the complete world alive through construction, access and destruction.
		// Use within one engine execution scope; TLS may suppress actual locking.
		// This guard neither retains the world nor validates body IDs.
		class WorldReadGuard
		{
		public:
			explicit WorldReadGuard(hknpBSWorld* a_world);
			~WorldReadGuard();

			WorldReadGuard(const WorldReadGuard&) = delete;
			WorldReadGuard(WorldReadGuard&&) = delete;
			WorldReadGuard& operator=(const WorldReadGuard&) = delete;
			WorldReadGuard& operator=(WorldReadGuard&&) = delete;

		private:
			hknpBSWorld*  _world{};  // 00
			std::uint32_t _mode{};   // 08
			std::uint32_t _pad0C{};  // 0C
		};
		static_assert(sizeof(WorldReadGuard) == 0x10);
		static_assert(alignof(WorldReadGuard) == 0x8);
	}
}
