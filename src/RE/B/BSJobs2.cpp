#include "RE/B/BSJobs2.h"

RE::BSJobs2::JobNode* RE::BSJobs2::AllocateJob(JobGroup* a_group, std::size_t a_size, std::size_t a_alignment, JobNode* a_prerequisite, JobNode* a_dependent)
{
	using func_t = decltype(&AllocateJob);
	static REL::Relocation<func_t> func{ ID::BSJobs2::AllocateJob };
	return func(a_group, a_size, a_alignment, a_prerequisite, a_dependent);
}
