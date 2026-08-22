#pragma once

#include "RE/B/BSTEvent.h"
#include "RE/B/BSTSingleton.h"

namespace RE
{
	class Actor;
	class BGSEquipSlot;
	class BGSObjectInstance;
	class TESBoundObject;

	namespace InventoryInterface
	{
		struct Handle;
	}

	namespace ActorEquipManagerEvent
	{
		struct Event;
		struct SpellEvent;
	}

	class ActorEquipManager :
		public BSTSingletonSDM<ActorEquipManager>,                 // 00
		public BSTEventSource<ActorEquipManagerEvent::Event>,      // 10
		public BSTEventSource<ActorEquipManagerEvent::SpellEvent>  // 38
	{
	public:
		[[nodiscard]] static ActorEquipManager* GetSingleton()
		{
			static REL::Relocation<ActorEquipManager**> singleton{ ID::ActorEquipManager::Singleton };
			return *singleton;
		}

		bool EquipObject(Actor* a_actor, const BGSObjectInstance& a_object, const BGSEquipSlot* a_slot, bool a_queueEquip, bool a_forceEquip, bool a_playSounds, bool a_applyNow, bool a_locked)
		{
			using func_t = decltype(&ActorEquipManager::EquipObject);
			static REL::Relocation<func_t> func{ ID::ActorEquipManager::EquipObject };
			return func(this, a_actor, a_object, a_slot, a_queueEquip, a_forceEquip, a_playSounds, a_applyNow, a_locked);
		}

		bool UseInventoryItem(Actor* a_actor, const InventoryInterface::Handle* a_handle, std::uint8_t a_result, bool a_allowUnequip, bool a_unk5, bool a_unk6)
		{
			using func_t = decltype(&ActorEquipManager::UseInventoryItem);
			static REL::Relocation<func_t> func{ ID::ActorEquipManager::UseInventoryItem };
			return func(this, a_actor, a_handle, a_result, a_allowUnequip, a_unk5, a_unk6);
		}

		bool UnequipObject(Actor* a_actor, const BGSObjectInstance& a_object, const BGSEquipSlot* a_slot, bool a_queueUnequip, bool a_forceUnequip, bool a_playSounds, bool a_applyNow, const BGSEquipSlot* a_slotBeingReplaced)
		{
			using func_t = decltype(&ActorEquipManager::UnequipObject);
			static REL::Relocation<func_t> func{ ID::ActorEquipManager::UnequipObject };
			return func(this, a_actor, a_object, a_slot, a_queueUnequip, a_forceUnequip, a_playSounds, a_applyNow, a_slotBeingReplaced);
		}
	};
	static_assert(sizeof(ActorEquipManager) == 0x60);
}
