#pragma once

namespace RE
{
	// Retains a native component view. Call on the game thread after gameplay initialization.
	class ViewSpace
	{
	public:
		ViewSpace() noexcept;
		~ViewSpace();
		ViewSpace(ViewSpace&& a_rhs) noexcept;
		ViewSpace& operator=(ViewSpace&& a_rhs) noexcept;
		ViewSpace(const ViewSpace&) = delete;
		ViewSpace& operator=(const ViewSpace&) = delete;
		[[nodiscard]] explicit operator bool() const noexcept;

		[[nodiscard]] static ViewSpace Capture();
		// Selects a system component view and synchronizes PlayerCamera ownership.
		// Coordinate cell and world loading separately.
		static void SelectSystem(std::uint32_t a_systemID);
		// Requires a valid captured view; this handle keeps its ownership afterward.
		void Select() const;

	private:
		void Release() noexcept;

		void* _space{};
	};
}
