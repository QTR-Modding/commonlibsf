#include "RE/S/SpaceshipDocking.h"

#include "RE/B/BSTArray.h"
#include "RE/N/NiTransform.h"
#include "RE/T/TESIdleForm.h"
#include "RE/T/TESObjectREFR.h"

namespace RE::Spaceship::Docking
{
	namespace
	{
		struct InteractionEntry
		{
			NiQuaternion  rotation;     // 00
			NiPoint3      translation;  // 10
			std::uint32_t unk1C;        // 1C
			TESIdleForm*  idle;         // 20 - borrowed
			std::byte     unk28[0x18];  // 28
		};
		static_assert(offsetof(TESIdleForm, conditions) == 0x48);
		static_assert(offsetof(InteractionEntry, translation) == 0x10);
		static_assert(offsetof(InteractionEntry, idle) == 0x20);
		static_assert(sizeof(InteractionEntry) == 0x40);

		struct InteractionGroup
		{
			std::uint32_t                  index;    // 00
			std::uint32_t                  unk04;    // 04
			BSScrapArray<InteractionEntry> entries;  // 08
		};
		static_assert(offsetof(InteractionGroup, entries) == 0x08);
		static_assert(sizeof(InteractionGroup) == 0x20);

		// The docking caller initializes these two references and zeroes the tail.
		struct DockConditionContext
		{
			TESObjectREFR* source;
			TESObjectREFR* target;
			std::byte      unk10[0x60]{};
		};
		static_assert(sizeof(DockConditionContext) == 0x70);

		struct StartSelection
		{
			StartPose     pose;
			std::uint32_t markerIndex;
		};

		// The native query copies DynamicState or BodyDraw and releases its own
		// database views. This output contains no owned references or allocations.
		struct NativeBodyPose
		{
			std::array<double, 4> position{};
			NiMatrix3             rotation;
			std::byte             stateFlag{};
			std::byte             pad51[0x0F]{};
			bool                  valid{};
			std::byte             pad61[0x0F]{};
		};
		static_assert(offsetof(NativeBodyPose, rotation) == 0x20);
		static_assert(offsetof(NativeBodyPose, stateFlag) == 0x50);
		static_assert(offsetof(NativeBodyPose, valid) == 0x60);
		static_assert(sizeof(NativeBodyPose) == 0x70);

		bool QueryBodyPose(TESObjectREFR* a_reference, NativeBodyPose& a_out)
		{
			using body_t = NativeBodyPose* (*)(NativeBodyPose*, TESObjectREFR*);
			static REL::Relocation<body_t> query{ ID::Spaceship::Docking::GetPhysicalBodyPose };
			query(std::addressof(a_out), a_reference);
			return a_out.valid;
		}

		std::array<double, 3> CurrentOrigin()
		{
			static REL::Relocation<const std::array<double, 3>*> floatingOrigin{ ID::Spaceship::Docking::FloatingOrigin };
			return *floatingOrigin.get();
		}

		bool QueryStartSelection(TESObjectREFR* a_source, TESObjectREFR* a_targetPort, StartSelection& a_out)
		{
			using query_t = bool (*)(TESObjectREFR*, void*, std::int32_t, bool, bool, void*, TESObjectREFR*, BSScrapArray<InteractionGroup>&, bool, bool, bool);
			static REL::Relocation<query_t> query{ ID::Spaceship::Docking::GetInteractionLocations };
			BSScrapArray<InteractionGroup>  groups;
			if (!query(a_source, nullptr, -1, false, false, nullptr, a_targetPort, groups, true, true, true) || groups.empty() || groups.front().entries.empty()) {
				return false;
			}

			const auto& entries = groups.front().entries;
			const auto* selected = std::addressof(entries.front());
			using condition_t = bool (*)(const TESCondition*, DockConditionContext*);
			static REL::Relocation<condition_t> conditionsPass{ ID::TESCondition::IsTrue };
			for (const auto& entry : entries) {
				if (entry.idle) {
					DockConditionContext context{ a_source, a_targetPort };
					if (conditionsPass(std::addressof(entry.idle->conditions), std::addressof(context))) {
						selected = std::addressof(entry);
						break;
					}
				}
			}
			a_out = { { selected->rotation, selected->translation }, groups.front().index };
			return true;
		}

		Pose QueryDockedPose(TESObjectREFR* a_source, TESObjectREFR* a_targetPort, std::uint32_t a_markerIndex)
		{
			NiTransform placement;
			using placement_t = bool (*)(TESObjectREFR*, std::uint32_t, NiTransform*);
			static REL::Relocation<placement_t> markerPlacement{ ID::Spaceship::Docking::GetMarkerPlacement };
			if (!markerPlacement(a_targetPort, a_markerIndex, std::addressof(placement))) {
				// BeginDock falls back to this port's raw transform without applying
				// the source docker offset when its furniture marker is unavailable.
				using rotation_t = void (*)(TESObjectREFR*, NiMatrix3*);
				static REL::Relocation<rotation_t> rotation{ ID::Spaceship::Docking::GetPortRotation };
				rotation(a_targetPort, std::addressof(placement.rotate));
				return { placement.rotate, a_targetPort->GetPosition() };
			}

			NiTransform inverseDocker;
			using inverse_t = bool (*)(TESObjectREFR*, TESObjectREFR*, NiTransform*, void*);
			static REL::Relocation<inverse_t> inverse{ ID::Spaceship::Docking::GetInverseDockerTransform };
			if (!inverse(a_source, a_targetPort, std::addressof(inverseDocker), nullptr)) {
				return { placement.rotate, placement.translate };
			}

			Pose docked;
			docked.rotation = inverseDocker.rotate * placement.rotate;
			for (std::size_t axis = 0; axis < 3; ++axis) {
				const float offset = inverseDocker.translate.z * placement.rotate[2][axis] + inverseDocker.translate.y * placement.rotate[1][axis] + inverseDocker.translate.x * placement.rotate[0][axis];
				docked.translation[axis] = offset * placement.scale + placement.translate[axis];
			}
			return docked;
		}

		Pose ComposeWorld(const Pose& a_local, const NativeBodyPose& a_target, const std::array<double, 3>& a_origin)
		{
			Pose world;
			world.rotation = a_local.rotation * a_target.rotation;
			// Match docking hierarchy composition in double precision, then the
			// physical-reference getter's conversion to current-origin movement XYZ.
			const double x = a_local.translation.x;
			const double y = a_local.translation.y;
			const double z = a_local.translation.z;
			for (std::size_t axis = 0; axis < 3; ++axis) {
				const double offset = y * a_target.rotation[1][axis] + x * a_target.rotation[0][axis] + z * a_target.rotation[2][axis];
				world.translation[axis] = static_cast<float>((offset + a_target.position[axis]) - a_origin[axis]);
			}
			return world;
		}
	}

	bool GetStartPose(TESObjectREFR* a_source, TESObjectREFR* a_targetPort, StartPose& a_out)
	{
		StartSelection selected;
		if (!QueryStartSelection(a_source, a_targetPort, selected)) {
			return false;
		}
		a_out = selected.pose;
		return true;
	}

	bool GetWorldPose(TESObjectREFR* a_ship, Pose& a_out)
	{
		NativeBodyPose body;
		if (!QueryBodyPose(a_ship, body)) {
			return false;
		}
		const auto origin = CurrentOrigin();
		Pose       result;
		result.rotation = body.rotation;
		for (std::size_t axis = 0; axis < 3; ++axis) {
			result.translation[axis] = static_cast<float>(body.position[axis] - origin[axis]);
		}
		a_out = result;
		return true;
	}

	bool GetWorldDockingPoses(TESObjectREFR* a_source, TESObjectREFR* a_target, TESObjectREFR* a_targetPort, WorldDockingPoses& a_out)
	{
		StartSelection selected;
		if (!QueryStartSelection(a_source, a_targetPort, selected)) {
			return false;
		}
		Pose start;
		selected.pose.rotation.ToMatrix(start.rotation);
		start.translation = selected.pose.translation;
		const auto docked = QueryDockedPose(a_source, a_targetPort, selected.markerIndex);

		NativeBodyPose target;
		if (!QueryBodyPose(a_target, target)) {
			return false;
		}
		const auto                           origin = CurrentOrigin();
		WorldDockingPoses                    result{ ComposeWorld(start, target, origin), ComposeWorld(docked, target, origin) };
		static REL::Relocation<const float*> facingDistance{ ID::Spaceship::Docking::FacingPointDistance };
		const float                          distance = *facingDistance.get();
		for (std::size_t axis = 0; axis < 3; ++axis) {
			result.targetOrigin[axis] = static_cast<float>(target.position[axis] - origin[axis]);
			result.facingPoint[axis] = result.start.translation[axis] + result.start.rotation[1][axis] * distance;
		}
		a_out = result;
		return true;
	}
}
