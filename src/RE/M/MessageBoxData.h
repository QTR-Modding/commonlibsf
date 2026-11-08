#pragma once

#include "RE/B/BSTArray.h"
#include "RE/M/MessageBox.h"

#include <array>

namespace RE::MessageBoxImpl
{
	[[nodiscard]] bool HasCallback(const void* a_info);
	void DestroyInfo(void* a_info);

	struct Button
	{
		explicit Button(const char* a_text);
		Button(Button&& a_other) noexcept;
		Button(const Button&) = delete;
		Button& operator=(Button&& a_other) noexcept;
		~Button();

		// Native text stores 12 inline bytes followed by capacity and length.
		std::array<std::uint32_t, 9> storage{ 0, 0, 0, 1, 0, 0, 0, 1, UINT32_MAX };
	};
	static_assert(sizeof(Button) == 0x24);

	struct Info
	{
		Info(const char* a_title, const char* a_text, const BSTSmartPointer<IMessageBoxCallback>& a_callback);
		Info(const Info&) = delete;
		~Info();

		alignas(8) std::array<std::byte, 0x40> storage{};
	};
	static_assert(sizeof(Info) == 0x40);

	struct Data
	{
		Data(Info& a_info, BSTArray<Button>& a_buttons, bool a_allowBackOut);
		Data(const Data&) = delete;
		~Data();
		[[nodiscard]] bool Queue();

		alignas(8) std::array<std::byte, 0x60> storage{};
	};
	static_assert(sizeof(Data) == 0x60);
}
