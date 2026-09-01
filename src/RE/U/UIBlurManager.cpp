#include "RE/U/UIBlurManager.h"
#include "RE/B/BlurEvent.h"

namespace RE
{
	namespace
	{
		void SendBlurEvent(const BlurEvent::Type a_type)
		{
			const BlurEvent event{ a_type };
			BlurEvent::GetEventSource()->Notify(event);
		}
	}

	void UIBlurManager::DecrementBlurCount()
	{
		SendBlurEvent(BlurEvent::Type::kDecrement);
	}

	void UIBlurManager::IncrementBlurCount()
	{
		SendBlurEvent(BlurEvent::Type::kIncrement);
	}
}
