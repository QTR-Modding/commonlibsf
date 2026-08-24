#pragma once

namespace RE
{
	class TESObjectREFR;

	namespace BSScript
	{
		class IVirtualMachine;
		class StaticFunctionTag;
	}

	namespace Game
	{
		inline void StartDialogueCameraOrCenterOnTarget(TESObjectREFR* a_cameraTarget = nullptr)
		{
			using func_t = void (*)(BSScript::IVirtualMachine*, std::uint32_t, BSScript::StaticFunctionTag*, TESObjectREFR*);
			static REL::Relocation<func_t> func{ ID::Game::StartDialogueCameraOrCenterOnTarget };
			func(nullptr, 0, nullptr, a_cameraTarget);
		}

		inline void StopDialogueCamera(bool a_considerResume = false, bool a_switchingToFirstPerson = false)
		{
			using func_t = void (*)(BSScript::IVirtualMachine*, std::uint32_t, BSScript::StaticFunctionTag*, bool, bool);
			static REL::Relocation<func_t> func{ ID::Game::StopDialogueCamera };
			func(nullptr, 0, nullptr, a_considerResume, a_switchingToFirstPerson);
		}
	}
}
