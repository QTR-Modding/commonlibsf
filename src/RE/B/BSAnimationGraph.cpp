#include "RE/B/BSAnimationGraph.h"

namespace RE
{
	namespace
	{
		struct AnimationSourceCollector
		{
			BSScrapArray<BSTEventSource<BSAnimationGraphEvent>*>* sources;
			bool                                                  found;
		};
		static_assert(offsetof(AnimationSourceCollector, found) == 0x8);
		static_assert(sizeof(AnimationSourceCollector) == 0x10);
	}

	std::vector<BSTEventSource<BSAnimationGraphEvent>*> BSAnimationGraphManager::GetAnimationEventSources()
	{
		BSScrapArray<BSTEventSource<BSAnimationGraphEvent>*> sources;
		AnimationSourceCollector                             collector{ std::addressof(sources), false };
		using func_t = void (*)(BSAnimationGraphManager*, AnimationSourceCollector*);
		static REL::Relocation<func_t> func{ ID::BSAnimationGraphManager::CollectAnimationEventSources };
		func(this, std::addressof(collector));
		return { sources.begin(), sources.end() };
	}
}
