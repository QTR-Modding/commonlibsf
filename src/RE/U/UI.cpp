#include "RE/U/UI.h"

#include "RE/T/TLS.h"
#include "REX/W32/KERNEL32.h"

namespace RE
{
	namespace
	{
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
