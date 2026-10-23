#pragma once

#include "RE/B/BGSEditorID.h"
#include "RE/B/BGSNavmeshableObject.h"
#include "RE/T/TESBoundObject.h"

namespace RE
{
	class BGSGenericBaseFormTemplate;

	class BGSGenericBaseForm :
		public TESBoundObject,       // 00
		public BGSNavmeshableObject  // E8
	{
	public:
		SF_RTTI_VTABLE(BGSGenericBaseForm);
		SF_FORMTYPE(GBFM);

		enum class RecordFlags : std::uint32_t
		{
			kSpaceship = 1u << 6
		};

		// Used by Papyrus SpaceshipReference classification; it does not require a body or AI.
		[[nodiscard]] bool IsSpaceship() const noexcept
		{
			return (formFlags.underlying() & std::to_underlying(RecordFlags::kSpaceship)) != 0;
		}

		// Broader native base-form predicate: spaceship flag OR spaceship-equipment component.
		// Equipment alone does not qualify references for Papyrus SpaceshipReference.
		[[nodiscard]] bool IsSpaceshipBase() const;

		// members
		BGSEditorID                 formEditorID;             // F0
		BGSGenericBaseFormTemplate* genericBaseFormTemplate;  // 100
	};
	static_assert(sizeof(BGSGenericBaseForm) == 0x108);
	static_assert(offsetof(BGSGenericBaseForm, formEditorID) == 0xF0);
	static_assert(offsetof(BGSGenericBaseForm, genericBaseFormTemplate) == 0x100);
}
