#include "RE/A/Actor.h"

void RE::Actor::RequestFurnitureExit()
{
	using func_t = void (*)(Actor*, bool, bool, bool);
	static REL::Relocation<func_t> func{ ID::Actor::RequestFurnitureExit };
	func(this, false, true, true);
}

bool RE::Actor::RequestNormalFurnitureExit()
{
	if (!currentProcess || !currentProcess->middleHigh) {
		return false;
	}
	using func_t = bool (*)(AIProcess*, Actor*);
	static REL::Relocation<func_t> func{ ID::AIProcess::RequestFurnitureExit };
	return func(currentProcess, this);
}
