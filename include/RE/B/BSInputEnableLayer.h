#pragma once

#include "RE/B/BSInputEnableManager.h"

namespace RE
{
	class BSInputEnableLayer
	{
	public:
		// Adapted from ozooma10/commonlibsf commit 6d819bcc371e8b0ff72511d25cdbd50a3330a6b8.
		inline void DecRef()
		{
			using func_t = decltype(&BSInputEnableLayer::DecRef);
			static REL::Relocation<func_t> func{ ID::BSInputEnableLayer::DecRef };
			func(this);
		}

		inline void EnableUserEvent(USER_EVENT_FLAG a_flags, bool a_enable, USER_EVENT_SENDER_ID a_sender = USER_EVENT_SENDER_ID::None)
		{
			BSInputEnableManager::EnableUserEvent(&m_layerID, a_flags, a_enable, a_sender);
		}

		inline void EnableOtherEvent(OTHER_EVENT_FLAG a_flags, bool a_enable, USER_EVENT_SENDER_ID a_sender = USER_EVENT_SENDER_ID::None)
		{
			BSInputEnableManager::EnableOtherEvent(&m_layerID, a_flags, a_enable, a_sender);
		}

		inline std::uint32_t GetLayerID() const
		{
			return m_layerID;
		}

		inline void IncRef()
		{
			REX::W32::InterlockedIncrement(&m_refCount);
		}

	private:
		// members
		std::uint32_t m_layerID;
		std::uint32_t m_refCount;
		std::uint64_t unk08;
		std::uint64_t unk10;
		std::uint64_t unk18;
	};
}
