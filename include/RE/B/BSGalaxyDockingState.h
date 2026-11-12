#pragma once

#include "RE/B/BSCoreTypes.h"
#include "RE/B/BSTArray.h"
#include "RE/N/NiMatrix3.h"

namespace RE::BSGalaxy
{
	// Component payload only; excludes the aligned 0x10-byte database record header.
	// The pose is relative to targetID, not an absolute world or raw reference pose.
	// Creating this state starts an attachment; it does not mean docking has finished.
	struct DockingChildState
	{
		NiMatrix3 rotation;           // 00
		std::byte unk30[0x10];        // 30
		double    translation[3];     // 40
		std::byte unk58[0x08];        // 58
		TESFormID targetID;           // 60
		TESFormID targetPortID;       // 64
		TESFormID sourcePortID;       // 68
		TESFormID associatedActorID;  // 6C
	};
	static_assert(offsetof(DockingChildState, translation) == 0x40);
	static_assert(offsetof(DockingChildState, targetID) == 0x60);
	static_assert(offsetof(DockingChildState, targetPortID) == 0x64);
	static_assert(offsetof(DockingChildState, sourcePortID) == 0x68);
	static_assert(offsetof(DockingChildState, associatedActorID) == 0x6C);
	static_assert(sizeof(DockingChildState) == 0x70);

	// This payload has an 0x08-byte database header, unlike DockingChildState.
	// The engine owns its child-ID array. Component/array access needs the database's
	// lifetime and synchronization guard; these layouts do not supply that guard.
	struct DockingParentState
	{
		BSTArray<TESFormID> children;  // 00
	};
	static_assert(sizeof(DockingParentState) == 0x10);
}
