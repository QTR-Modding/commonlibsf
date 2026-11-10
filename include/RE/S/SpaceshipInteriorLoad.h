#pragma once

#include "RE/N/NiSmartPointer.h"

namespace RE
{
	class TESObjectREFR;
}

namespace RE::Spaceship
{
	// Game-thread ownership of an additive linked-interior request.
	// Existing loading or ready resources remain owned by their original caller.
	class InteriorLoad
	{
	public:
		enum class Status
		{
			Pending,
			Attached,
			Failed,
			Replaced
		};

		InteriorLoad() noexcept;
		// Destruction and move assignment abandon requests that Reset cannot release yet.
		~InteriorLoad();
		InteriorLoad(InteriorLoad&& a_rhs) noexcept;
		InteriorLoad& operator=(InteriorLoad&& a_rhs) noexcept;
		InteriorLoad(const InteriorLoad&) = delete;
		InteriorLoad& operator=(const InteriorLoad&) = delete;
		[[nodiscard]] explicit operator bool() const noexcept;
		[[nodiscard]] Status GetStatus() const;

		// Schedules native loading and attachment; coordinate the loading lifecycle separately.
		// A retained request can still be loading or failed. Check GetStatus before use.
		[[nodiscard]] static InteriorLoad Request(TESObjectREFR* a_ship);
		// Pending owned work cannot be interrupted. False preserves this request unchanged.
		[[nodiscard]] bool Reset();
		// Accepted game loads and shutdown retain responsibility for native scene cleanup.
		void Abandon() noexcept;

	private:
		[[nodiscard]] Status GetResourceStatus() const;
		static void* CaptureResource(TESObjectREFR* a_ship);
		static void ReleaseResource(void* a_resource) noexcept;

		NiPointer<TESObjectREFR> _ship;
		void* _resource{};
		bool _ownsLoad{};
	};
}
