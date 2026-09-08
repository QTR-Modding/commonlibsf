#pragma once

#include "RE/I/IDataModel.h"

namespace RE
{
	namespace InventoryInterface
	{
		struct Handle;
	}

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

		[[nodiscard]] bool QueueRemoveItem(const InventoryInterface::Handle& a_handle)
		{
			if (!unk08) {
				return false;
			}

			using func_t = void (*)(void*, const InventoryInterface::Handle*);
			static REL::Relocation<func_t> func{ ID::PlayerInventoryDataModel::QueueRemoveItem };
			func(unk08, std::addressof(a_handle));
			return true;
		}
	};
}
