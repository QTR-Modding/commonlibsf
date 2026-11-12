#pragma once

#include "RE/I/IDataModel.h"
#include "REL/Relocation.h"
#include "RE/IDs.h"

#include <cstdint>

namespace RE
{
	class DialogueUIDataModel :
		public IDataModel
	{
	public:
		SF_RTTI_VTABLE(DialogueUIDataModel);

		[[nodiscard]] static DialogueUIDataModel* GetSingleton()
		{
			static REL::Relocation<DialogueUIDataModel**> singleton{ ID::DialogueUIDataModel::Singleton };
			return *singleton;
		}

		// Rebuild current dialogue choices on the UI thread.
		void RefreshChoices()
		{
			using func_t = std::uint64_t (*)(void*, DialogueUIDataModel*);
			static REL::Relocation<func_t> func{ ID::DialogueUIDataModel::RefreshChoices };
			func(nullptr, this);
		}
	};
}
