#pragma once

#include "RE/B/BSJobs2.h"

namespace RE::Spaceship::Systems
{
	// Adds the ordinary eligible ship-system jobs to this frame's native graph.
	// Call during graph construction, before its jobs are submitted.
	void QueueSystemUpdates(const BSJobs2::JobContext& a_context);
}
