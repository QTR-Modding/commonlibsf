#include "RE/A/Actor.h"

void RE::Actor::RequestFurnitureExit()
{
	using func_t = void (*)(Actor*, bool, bool, bool);
	static REL::Relocation<func_t> func{ ID::Actor::RequestFurnitureExit };
	func(this, false, true, true);
}
