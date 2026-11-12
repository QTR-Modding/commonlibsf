#pragma once

namespace RE
{
	class ViewSpaceOriginScope
	{
	public:
		explicit ViewSpaceOriginScope(void* a_view);
		~ViewSpaceOriginScope();
		ViewSpaceOriginScope(const ViewSpaceOriginScope&) = delete;
		ViewSpaceOriginScope& operator=(const ViewSpaceOriginScope&) = delete;
		[[nodiscard]] std::byte* GetData() const;

	private:
		struct Cursor
		{
			std::array<std::uintptr_t, 4> pages{};
			std::array<std::uint32_t, 4> indices{};
			std::uint32_t depth{};
			std::uint32_t flags{};
			void* view{};
		};
		static_assert(sizeof(Cursor) == 0x40);
		static_assert(offsetof(Cursor, depth) == 0x30);
		void* _view{};
	};
}
