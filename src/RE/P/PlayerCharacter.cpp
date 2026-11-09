#include "RE/P/PlayerCharacter.h"

void RE::PlayerCharacter::SetFirstPerson3D(bool a_firstPerson)
{
	using func_t = void (*)(void*, bool);
	static REL::Relocation<func_t> func{ ID::PlayerCharacter::SetFirstPerson3D };
	func(nullptr, a_firstPerson);
}
