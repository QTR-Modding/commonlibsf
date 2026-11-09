#include "RE/B/BGSSaveLoad.h"

bool RE::BGSSaveLoadManager::IsSavingAllowed(bool a_ignorePlayerStateChecks)
{
	using func_t = bool (*)(BGSSaveLoadManager*, bool);
	static REL::Relocation<func_t> func{ ID::BGSSaveLoadManager::IsSavingAllowed };
	return func(this, a_ignorePlayerStateChecks);
}
