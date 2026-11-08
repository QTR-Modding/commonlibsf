#pragma once

#include "RE/B/BSIntrusiveRefCounted.h"

namespace RE
{
	class __declspec(novtable) IMessageBoxCallback : public BSIntrusiveRefCounted
	{
	public:
		SF_RTTI_VTABLE(IMessageBoxCallback);

		constexpr static std::uint8_t kCancelled = 0xFF;

		virtual ~IMessageBoxCallback();
		virtual void Run(std::uint8_t a_button) = 0;
	};
	static_assert(sizeof(IMessageBoxCallback) == 0x10);
}
