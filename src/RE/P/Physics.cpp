#include "RE/P/Physics.h"

void RE::Physics::IncludeSpaceWorld(WorldCollection& a_worlds)
{
	static REL::Relocation<bhkWorld**> spaceWorld{ ID::Physics::SpaceWorld };
	bhkWorld* world = *spaceWorld;
	if (!world) {
		return;
	}
	using find_t = std::int32_t (*)(const WorldCollection*, bhkWorld* const*);
	static REL::Relocation<find_t> find{ ID::Physics::FindWorld };
	if (find(std::addressof(a_worlds), std::addressof(world)) != -1) {
		return;
	}
	using append_t = bhkWorld** (*)(WorldCollection*);
	static REL::Relocation<append_t> append{ ID::Physics::AppendWorld };
	*append(std::addressof(a_worlds)) = world;
}
