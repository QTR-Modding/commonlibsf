#include "RE/U/UI.h"

#include "RE/B/BSService.h"
#include "RE/T/TLS.h"
#include "REX/W32/KERNEL32.h"

namespace RE
{
	namespace
	{
		using UICommand = BSService::Detail::TCmd<std::move_only_function<void()>, void>;
		static_assert(offsetof(UICommand, executeAndDestroy) == 0x08);
		static_assert(offsetof(UICommand, function) == 0x10);
		static_assert(std::is_nothrow_constructible_v<UICommand, std::move_only_function<void()>&&>);

		struct UICommandBlock
		{
			UICommandBlock*         next;            // 00
			std::uint32_t           reservedBytes;   // 08
			volatile std::uint32_t readyBytes;      // 0C
			volatile std::uint32_t lastOffset;      // 10
			std::uint32_t           firstOffset;     // 14
			std::byte               pad18[0x28];     // 18
		};
		static_assert(offsetof(UICommandBlock, readyBytes) == 0x0C);
		static_assert(offsetof(UICommandBlock, lastOffset) == 0x10);
		static_assert(offsetof(UICommandBlock, firstOffset) == 0x14);
		static_assert(sizeof(UICommandBlock) == 0x40);

		struct UICommandEntry
		{
			std::uint32_t reservedBytes;  // 00
			std::uint32_t nextOffset;     // 04
		};
		static_assert(sizeof(UICommandEntry) == 0x08);

		struct UICommandAllocation
		{
			UICommandBlock* block;          // 00
			std::uint32_t   offset;         // 08
			std::uint32_t   reservedBytes;  // 0C
		};
		static_assert(offsetof(UICommandAllocation, offset) == 0x08);
		static_assert(offsetof(UICommandAllocation, reservedBytes) == 0x0C);
		static_assert(sizeof(UICommandAllocation) == 0x10);

		struct UIMenuArrayLock
		{
			volatile std::uint32_t state;           // 00
			std::uint32_t          unk04;           // 04
			std::uint32_t          unk08;           // 08
			volatile std::uint32_t writerThreadID;  // 0C
		};
		static_assert(sizeof(UIMenuArrayLock) == 0x10);

		[[nodiscard]] std::uint32_t GetLogicalThreadID()
		{
			auto* tls = TLS::GetSingleton();
			auto  threadID = tls->logicalThreadID;
			if (threadID == 0) {
				static REL::Relocation<volatile std::uint32_t*> counter{ ID::TLS::LogicalThreadIDCounter };
				threadID = static_cast<std::uint32_t>(
					_InterlockedExchangeAdd(reinterpret_cast<volatile long*>(counter.get()), 1));
				tls->logicalThreadID = threadID;
			}
			return threadID;
		}

		class UIMenuArrayReadGuard
		{
		public:
			explicit UIMenuArrayReadGuard(UIMenuArrayLock& a_lock) :
				lock(a_lock)
			{
				const auto threadID = GetLogicalThreadID();
				if (lock.writerThreadID == threadID) {
					return;
				}

				auto state = static_cast<std::uint32_t>(
					_InterlockedExchangeAdd(reinterpret_cast<volatile long*>(std::addressof(lock.state)), 4));
				std::uint32_t spinCount = 0;
				while ((state & 0x2u) != 0) {
					if (spinCount == 0) {
						_mm_pause();
						spinCount = 1;
					} else {
						const bool sleepOneMillisecond = spinCount > 9999;
						if (!sleepOneMillisecond) {
							++spinCount;
						}
						REX::W32::Sleep(static_cast<std::uint32_t>(sleepOneMillisecond));
					}
					state = lock.state;
				}
			}

			~UIMenuArrayReadGuard()
			{
				const auto threadID = GetLogicalThreadID();
				if (lock.writerThreadID != threadID) {
					_InterlockedExchangeAdd(
						reinterpret_cast<volatile long*>(std::addressof(lock.state)), -4);
				}
			}

			UIMenuArrayReadGuard(const UIMenuArrayReadGuard&) = delete;
			UIMenuArrayReadGuard& operator=(const UIMenuArrayReadGuard&) = delete;

		private:
			UIMenuArrayLock& lock;
		};

		[[nodiscard]] UIMenuArrayLock& GetMenuArrayLock()
		{
			static REL::Relocation<UIMenuArrayLock*> lock{ ID::UI::MenuArrayLock };
			return *lock;
		}
	}

	void UI::QueueTask(std::move_only_function<void()> a_task)
	{
		constexpr std::ptrdiff_t commandQueueOffset = 0x200;
		constexpr std::ptrdiff_t commandCapacityOffset = 0x364;
		auto* queue = reinterpret_cast<std::byte*>(this) + commandQueueOffset;
		const auto capacity = *reinterpret_cast<const std::uint32_t*>(
			reinterpret_cast<const std::byte*>(this) + commandCapacityOffset);

		// All potentially throwing callable construction precedes the native reservation.
		using allocate_t = void* (*)(void*, UICommandAllocation*, std::uint32_t, std::uint32_t);
		static REL::Relocation<allocate_t> allocate{ ID::BSService::CommandQueue::Allocate };
		UICommandAllocation allocation;
		auto* storage = allocate(queue, std::addressof(allocation), sizeof(UICommand), alignof(UICommand));
		::new (storage) UICommand(std::move(a_task));

		// Match the native multi-producer publication: exchange the tail, link it, then commit bytes.
		auto* block = allocation.block;
		const auto previous = static_cast<std::uint32_t>(_InterlockedExchange(
			reinterpret_cast<volatile long*>(std::addressof(block->lastOffset)), allocation.offset));
		if (previous < capacity) {
			auto* entry = reinterpret_cast<UICommandEntry*>(
				reinterpret_cast<std::byte*>(block) + sizeof(UICommandBlock) + previous);
			entry->nextOffset = allocation.offset;
		} else {
			block->firstOffset = allocation.offset;
		}
		_InterlockedExchangeAdd(
			reinterpret_cast<volatile long*>(std::addressof(block->readyBytes)), allocation.reservedBytes);
	}

	Scaleform::Ptr<IMenu> UI::GetTopInputMenu() const
	{
		Scaleform::Ptr<IMenu> result;
		{
			UIMenuArrayReadGuard guard{ GetMenuArrayLock() };
			if (!menuArray.empty()) {
				result = menuArray.back();
			}
		}
		return result;
	}
}
