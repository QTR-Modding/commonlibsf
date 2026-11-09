#pragma once

#include "RE/B/BGSEditorID.h"
#include "RE/B/BGSNavmeshableObject.h"
#include "RE/T/TESBoundObject.h"

#include <vector>

namespace RE
{
	class BGSGenericBaseFormTemplate;
	class TESObjectCELL;

	class BGSGenericBaseForm :
		public TESBoundObject,       // 00
		public BGSNavmeshableObject  // E8
	{
	public:
		SF_RTTI_VTABLE(BGSGenericBaseForm);
		SF_FORMTYPE(GBFM);

		// Returns retained blueprint cells without loading their scenes.
		[[nodiscard]] std::vector<NiPointer<TESObjectCELL>> GetBlueprintCells() const;

		// members
		BGSEditorID                 formEditorID;             // E8
		BGSGenericBaseFormTemplate* genericBaseFormTemplate;  // F8
	};
	static_assert(sizeof(BGSGenericBaseForm) == 0x108);
}
