#pragma once

#include "RE/B/BSLock.h"
#include "RE/B/BSTArray.h"
#include "RE/B/BSTSingleton.h"
#include "RE/I/IDataModel.h"
#include "RE/P/PlayerInventoryDataModel.h"
#include "RE/RTTI.h"

namespace RE
{
	class GameUIModel :
		public BSTSingletonSDM<GameUIModel>  // 00
	{
	public:
		SF_RTTI_VTABLE(GameUIModel);

		[[nodiscard]] static GameUIModel* GetSingleton()
		{
			static REL::Relocation<GameUIModel**> singleton{ ID::GameUIModel::Singleton };
			return *singleton;
		}

		// Finds and invokes the session-owned model without allowing its pointer to escape
		// the engine's lifetime lock.
		[[nodiscard]] static bool ReconcilePlayerInventory(bool a_incremental)
		{
			static REL::Relocation<BSReadWriteLock*> lock{ ID::GameUIModel::DataModelLock };
			const BSAutoReadLock guard{ lock.get() };

			const auto singleton = GetSingleton();
			if (!singleton || !singleton->initialized) {
				return false;
			}

			for (const auto model : singleton->ownedModels) {
				if (const auto inventory = starfield_cast<PlayerInventoryDataModel*>(model)) {
					inventory->Reconcile(a_incremental);
					return true;
				}
			}

			return false;
		}

		// members
		IDataModel*            unk10;           // 10
		bool                   initialized;     // 18
		std::byte              pad19[0x7];      // 19
		BSTArray<IDataModel*> externalModels;  // 20
		BSTArray<IDataModel*> ownedModels;     // 30
	};
	static_assert(offsetof(GameUIModel, externalModels) == 0x20);
	static_assert(offsetof(GameUIModel, ownedModels) == 0x30);
	static_assert(sizeof(GameUIModel) == 0x40);
}
