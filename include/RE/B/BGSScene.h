#pragma once

#include "RE/I/IKeywordFormBase.h"
#include "RE/T/TESForm.h"

namespace RE
{
	class BGSScene :
		public TESForm,          // 00
		public IKeywordFormBase  // 30
	{
	public:
		SF_RTTI_VTABLE(BGSScene);
		SF_FORMTYPE(SCEN);

		// The parent quest must be running.
		void ForceStart()
		{
			using func_t = decltype(&BGSScene::ForceStart);
			static REL::Relocation<func_t> func{ ID::BGSScene::ForceStart };
			func(this);
		}
	};
}
