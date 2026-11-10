#pragma once

namespace RE::BSJobs2
{
	class JobGroup;
	class JobNode;

	// Borrowed nodes in a native frame graph under construction.
	struct JobContext
	{
		JobGroup* group;
		JobNode* prerequisite;
		JobNode* dependent;
	};

	// The group owns the allocation and both dependency edges.
	[[nodiscard]] JobNode* AllocateJob(JobGroup* a_group, std::size_t a_size, std::size_t a_alignment, JobNode* a_prerequisite, JobNode* a_dependent);
}
