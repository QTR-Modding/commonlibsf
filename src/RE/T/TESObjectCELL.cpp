#include "RE/T/TESObjectCELL.h"

namespace RE
{
	NiPointer<TESObjectREFR> TESObjectCELL::GetParentRef() const
	{
		using Handle = BSPointerHandle<TESObjectREFR>;
		using func_t = Handle* (*)(Handle*, void*, const TESObjectCELL*);
		static REL::Relocation<func_t> func{ ID::TESObjectCELL::GetParentRef };
		Handle                         handle;
		func(std::addressof(handle), nullptr, this);
		return handle.get();
	}
}
