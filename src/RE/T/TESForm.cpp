#include "RE/T/TESForm.h"

RE::TESForm* RE::TESForm::GetLinkedForm(BGSKeyword* a_keyword)
{
	struct FormLinkView
	{
		void*          data{};
		std::uintptr_t scope{};
	};
	using acquire_t = FormLinkView* (*)(TESForm*, FormLinkView*);
	using find_t = TESForm* (*)(const void*, BGSKeyword*);
	using release_t = void (*)(std::uintptr_t*);
	static REL::Relocation<acquire_t> acquire{ ID::TESForm::GetFormLinkData };
	static REL::Relocation<find_t> find{ ID::TESForm::FindFormLink };
	static REL::Relocation<release_t> release{ ID::TESForm::ReleaseFormLinkData };

	FormLinkView view;
	acquire(this, &view);
	TESForm* result = view.data ? find(view.data, a_keyword) : nullptr;
	release(&view.scope);
	return result;
}
