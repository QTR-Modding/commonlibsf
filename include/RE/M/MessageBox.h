#pragma once

#include "RE/B/BSTSmartPointer.h"
#include "RE/I/IMessageBoxCallback.h"

#include <span>

namespace RE
{
	// Requires a nonempty callback. Returns true when the native queue accepts and
	// retains the request. Rejection returns false without invoking the callback.
	// Copies the title, text and buttons. Selection is zero-based; dismissal reports
	// kCancelled. The callback runs before the menu closes or advances to its next box.
	[[nodiscard]] bool ShowMessageBox(const char* a_title, const char* a_text, std::span<const char* const> a_buttons,
		const BSTSmartPointer<IMessageBoxCallback>& a_callback, bool a_allowBackOut = true);
}
