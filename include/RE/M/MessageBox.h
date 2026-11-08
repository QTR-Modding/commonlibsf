#pragma once

#include "RE/B/BSTSmartPointer.h"
#include "RE/I/IMessageBoxCallback.h"

#include <span>

namespace RE
{
	// Copies the text and buttons into the native modal message queue and retains
	// the callback. Selection is zero-based; dismissal reports kCancelled.
	// The callback runs before the native menu closes or advances to its next box.
	void ShowMessageBox(const char* a_title, const char* a_text, std::span<const char* const> a_buttons,
		const BSTSmartPointer<IMessageBoxCallback>& a_callback, bool a_allowBackOut = true);
}
