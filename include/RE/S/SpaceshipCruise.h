#pragma once

#include <cstdint>

namespace RE::Spaceship::Cruise
{
	// Native cruise operates on the player's current containing ship. Call on the game
	// thread after gameplay singletons are initialized; mutators require a valid ship.

	// Tests normal native eligibility, including the player's pilot-role requirement.
	[[nodiscard]] bool CanStart();
	[[nodiscard]] bool IsActive();

	// Returns the normalized celestial-body ID of the locked course, or zero.
	[[nodiscard]] std::uint32_t GetCourse();

	// Normalizes reference IDs to their celestial body. Zero or the current course ID
	// clears the course; any other destination selects it. This is not an idempotent setter.
	void ToggleCourse(std::uint32_t a_destinationID);

	// Normal entry checks eligibility and seeds minimum target throttle when current throttle is lower.
	// Restoring state bypasses eligibility, proximity initialization and throttle seeding,
	// and suppresses the initial entry effect. The native call reports no success value.
	void Start(bool a_restoreState = false);

	// Requests the normal braking/arrival sequence at the current ship location.
	// Requires active cruise; completion is asynchronous and does not require a HUD.
	void RequestExit();

	// Immediately clears native cruise state, course and effects without requiring a HUD.
	// This is cleanup, rather than a request for the normal exit/arrival sequence.
	void Stop();
}
