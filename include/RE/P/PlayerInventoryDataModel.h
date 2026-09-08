#pragma once

#include "RE/I/IDataModel.h"

namespace RE
{
	class PlayerInventoryDataModel :
		public IDataModel  // 000
	{
	public:
		SF_RTTI_VTABLE(PlayerInventoryDataModel);

		void Reconcile(bool a_incremental)
		{
			using func_t = decltype(&PlayerInventoryDataModel::Reconcile);
			static REL::Relocation<func_t> func{ ID::PlayerInventoryDataModel::Reconcile };
			func(this, a_incremental);
		}
	};
}
