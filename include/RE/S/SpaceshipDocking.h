#pragma once

#include "RE/N/NiMatrix3.h"
#include "RE/N/NiQuaternion.h"

namespace RE
{
	class TESObjectREFR;

	namespace Spaceship::Docking
	{
		// Call these native entry points on the game thread with valid, non-null references.
		struct StartPose
		{
			NiQuaternion rotation;
			NiPoint3     translation;
		};

		struct Pose
		{
			NiMatrix3 rotation;
			NiPoint3  translation;
		};

		struct WorldDockingPoses
		{
			Pose     start;
			Pose     docked;
			NiPoint3 targetOrigin;
			NiPoint3 facingPoint;
		};

		// Native XY target-bounds allowance used by HUD distance and docking range,
		// in the same units as fSpaceshipMaxDockingDistance.
		[[nodiscard]] float GetTargetRangeAllowance(TESObjectREFR* a_target);

		// Selects a compatible target port for the source ship. The returned reference is borrowed.
		[[nodiscard]] TESObjectREFR* FindPort(TESObjectREFR* a_source, TESObjectREFR* a_target);

		// Copies vanilla's initial interaction pose relative to the target ship (the docking parent).
		// Compose with the target's current body transform before flight; this is not an absolute pose.
		// Does not move the ship; failure leaves a_out unchanged.
		// Requires a valid source ship and a port returned by FindPort for that ship/target pair.
		[[nodiscard]] bool GetStartPose(TESObjectREFR* a_source, TESObjectREFR* a_targetPort, StartPose& a_out);

		// Copies the current physical body pose in current-origin common XYZ.
		// Rotation rows are world-space axes. Failure leaves a_out unchanged.
		[[nodiscard]] bool GetWorldPose(TESObjectREFR* a_ship, Pose& a_out);

		// Copies the animation start and final docked poses using one target-body
		// snapshot, with the target body origin and the stock start-facing point.
		// Translations and points use current-origin common XYZ for native movement;
		// rotation rows are world-space axes. Does not move the ship.
		// Requires a compatible source/target/port pair. Failure leaves a_out unchanged.
		[[nodiscard]] bool GetWorldDockingPoses(TESObjectREFR* a_source, TESObjectREFR* a_target, TESObjectREFR* a_targetPort, WorldDockingPoses& a_out);

		// Runs vanilla's final docking action with valid ship references and their selected port.
		// True means the action was accepted, not completed. The player-associated branch can
		// reposition the ship and start the vanilla docking camera; this wrapper does not suppress it.
		// A rejected action can already have submitted alignment movement.
		// A false result can already have submitted movement; do not use this call as a query.
		[[nodiscard]] bool BeginDock(TESObjectREFR* a_source, TESObjectREFR* a_target, TESObjectREFR* a_targetPort);
	}
}
