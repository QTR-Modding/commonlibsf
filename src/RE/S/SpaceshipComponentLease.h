#pragma once

#include "RE/IDs.h"
#include "REL/Relocation.h"

#include <atomic>
#include <cstddef>
#include <cstdint>

namespace RE::Spaceship::detail
{
	// Native component getters transfer a paired reference/weak lease.
	template <class T>
	struct ComponentLease
	{
		T* value{};
		std::byte* owner{};

		ComponentLease() = default;
		ComponentLease(const ComponentLease&) = delete;
		ComponentLease& operator=(const ComponentLease&) = delete;
		~ComponentLease()
		{
			if (owner) {
				std::atomic_ref counts{ *reinterpret_cast<std::uint64_t*>(owner - 0x20) };
				if (counts.fetch_sub(0x100000001ULL) == 0x100000001ULL) {
					using func_t = void (*)(void*);
					static REL::Relocation<func_t> func{ ID::Spaceship::Movement::ReleaseControllerLease };
					func(owner - 0x20);
				}
			}
		}
	};
	static_assert(sizeof(ComponentLease<void>) == 0x10);
}
