#include "RE/S/SpaceshipPower.h"
#include "RE/T/TESObjectREFR.h"
#include "SpaceshipComponentLease.h"

namespace RE::Spaceship::Power
{
	namespace
	{
		struct PartView
		{
			std::byte       unk00[0x10];
			TESObjectREFR*  ship;
			void*           weapon;
			std::byte       unk20[0x8];
			ActorValueInfo* allocation;
			std::byte       unk30[0x8];
			ActorValueInfo* bonus;
			std::byte       unk40[0x20];
			std::int32_t    maximum;
			std::int32_t    allocated;
			std::int32_t    damagePhysical;
			std::int32_t    damageEM;
		};
		static_assert(offsetof(PartView, ship) == 0x10);
		static_assert(offsetof(PartView, allocation) == 0x28);
		static_assert(offsetof(PartView, bonus) == 0x38);
		static_assert(offsetof(PartView, maximum) == 0x60);
		static_assert(offsetof(PartView, allocated) == 0x64);
		static_assert(sizeof(PartView) == 0x70);

		struct EquipmentView
		{
			std::byte     unk00[0x30];
			std::uint32_t count;
			std::byte     unk34[0x4];
			PartView**    parts;
			std::byte     unk40[0xA0];
			std::int32_t  available;
		};
		static_assert(offsetof(EquipmentView, count) == 0x30);
		static_assert(offsetof(EquipmentView, parts) == 0x38);
		static_assert(offsetof(EquipmentView, available) == 0xE0);

		using EquipmentLease = detail::ComponentLease<EquipmentView>;

		void GetEquipment(TESObjectREFR* a_ship, EquipmentLease& a_lease)
		{
			using func_t = EquipmentLease* (*)(EquipmentLease*, TESFormID);
			static REL::Relocation<func_t> func{ ID::Spaceship::Power::GetEquipment };
			func(&a_lease, a_ship->GetFormID());
		}

		bool Identify(const PartView& a_part, System& a_system, std::uint32_t& a_group)
		{
			if (!a_part.allocation) {
				return false;
			}
			a_group = 0;
			if (a_part.weapon) {
				using func_t = ActorValueInfo* (*)(std::uint32_t);
				static REL::Relocation<func_t> func{ ID::Spaceship::Power::GetWeaponPowerActorValue };
				for (std::uint32_t group = 0; group < 3; ++group) {
					if (a_part.allocation == func(group)) {
						a_group = group;
						a_system = System::Weapon;
						return true;
					}
				}
				return false;
			}
			static REL::Relocation<ActorValueInfo**> engine{ ID::Spaceship::Power::EnginePower };
			static REL::Relocation<ActorValueInfo**> shield{ ID::Spaceship::Power::ShieldPower };
			static REL::Relocation<ActorValueInfo**> grav{ ID::Spaceship::Power::GravDrivePower };
			if (a_part.allocation == *engine) {
				a_system = System::Engine;
			} else if (a_part.allocation == *shield) {
				a_system = System::Shield;
			} else if (a_part.allocation == *grav) {
				a_system = System::GravDrive;
			} else {
				return false;
			}
			return true;
		}
	}

	bool Read(TESObjectREFR* a_ship, Snapshot& a_snapshot)
	{
		a_snapshot = {};
		EquipmentLease equipment;
		GetEquipment(a_ship, equipment);
		if (!equipment.value) {
			return false;
		}
		a_snapshot.available = equipment.value->available;
		static REL::Relocation<ActorValueInfo**> maximum{ ID::Spaceship::Power::MaxAvailablePower };
		if (*maximum) {
			a_snapshot.maximum = static_cast<std::int32_t>(a_ship->GetActorValue(**maximum) -
														   a_ship->GetModifier(ACTOR_VALUE_MODIFIER::kDamage, **maximum));
		}
		for (std::uint32_t i = 0; i < equipment.value->count; ++i) {
			const PartView& part = *equipment.value->parts[i];
			Component       component{};
			if (part.ship != a_ship || !Identify(part, component.system, component.weaponGroup)) {
				continue;
			}
			component.allocated = part.allocated;
			component.bonus = part.bonus ? static_cast<std::int32_t>(part.ship->GetActorValue(*part.bonus)) : 0;
			component.maximum = part.maximum;
			component.damagePhysical = part.damagePhysical;
			component.damageEM = part.damageEM;
			a_snapshot.components.push_back(component);
		}
		return true;
	}

	bool Change(TESObjectREFR* a_ship, System a_system, std::uint32_t a_weaponGroup, std::int32_t a_delta)
	{
		if (a_delta != -1 && a_delta != 1) {
			return false;
		}
		EquipmentLease equipment;
		GetEquipment(a_ship, equipment);
		if (!equipment.value) {
			return false;
		}
		for (std::uint32_t i = 0; i < equipment.value->count; ++i) {
			PartView&     part = *equipment.value->parts[i];
			System        system;
			std::uint32_t group;
			if (part.ship != a_ship || !Identify(part, system, group) || system != a_system ||
				(system == System::Weapon && group != a_weaponGroup)) {
				continue;
			}
			const auto before = part.allocated;
			using func_t = void (*)(EquipmentView*, PartView*, std::int32_t, std::uint8_t);
			static REL::Relocation<func_t> func{ ID::Spaceship::Power::SetAllocation };
			func(equipment.value, &part, before + a_delta, 1);
			return part.allocated != before;
		}
		return false;
	}
}
