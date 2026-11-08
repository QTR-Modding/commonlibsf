#include "RE/M/MessageBox.h"
#include "MessageBoxData.h"

RE::IMessageBoxCallback::~IMessageBoxCallback() = default;

RE::MessageBoxImpl::Button::Button(const char* a_text)
{
	using func_t = void (*)(void*, const char*, bool);
	static REL::Relocation<func_t> assign{ ID::MessageBox::AssignText };
	assign(storage.data(), "", false);
	assign(storage.data() + 4, a_text, false);
}

RE::MessageBoxImpl::Button::Button(Button&& a_other) noexcept
{
	using func_t = void (*)(void*, void*);
	static REL::Relocation<func_t> move{ ID::MessageBox::MoveText };
	move(storage.data(), a_other.storage.data());
	move(storage.data() + 4, a_other.storage.data() + 4);
	storage[8] = a_other.storage[8];
}

RE::MessageBoxImpl::Button& RE::MessageBoxImpl::Button::operator=(Button&& a_other) noexcept
{
	if (this != std::addressof(a_other)) {
		std::destroy_at(this);
		std::construct_at(this, std::move(a_other));
	}
	return *this;
}
RE::MessageBoxImpl::Button::~Button()
{
	using func_t = void (*)(Button*);
	static REL::Relocation<func_t> destroy{ ID::MessageBox::DestroyButton };
	destroy(this);
}

RE::MessageBoxImpl::Info::Info(const char* a_title, const char* a_text,
	const BSTSmartPointer<IMessageBoxCallback>& a_callback)
{
	using func_t = Info* (*)(Info*, const char*, const char*, const BSTSmartPointer<IMessageBoxCallback>*,
		std::uint64_t, const char*);
	static REL::Relocation<func_t> construct{ ID::MessageBox::ConstructInfo };
	construct(this, a_title, a_text, std::addressof(a_callback), 0, nullptr);
}

RE::MessageBoxImpl::Info::~Info()
{
	using func_t = void (*)(void*);
	static REL::Relocation<func_t> destroy{ ID::MessageBox::DestroyInfo };
	destroy(this);
}

RE::MessageBoxImpl::Data::Data(Info& a_info, BSTArray<Button>& a_buttons, bool a_allowBackOut)
{
	using func_t = Data* (*)(Data*, Info*, BSTArray<Button>*, std::uint32_t, bool, bool, bool, std::uint32_t);
	static REL::Relocation<func_t> construct{ ID::MessageBox::ConstructData };
	construct(this, std::addressof(a_info), std::addressof(a_buttons), 10, true, false, a_allowBackOut, UINT32_MAX);
}

RE::MessageBoxImpl::Data::~Data()
{
	using func_t = void (*)(void*);
	static REL::Relocation<func_t> destroyInfo{ ID::MessageBox::DestroyInfo };
	static REL::Relocation<func_t> destroyButtons{ ID::MessageBox::DestroyButtons };
	destroyButtons(storage.data() + 0x40);
	destroyInfo(this);
}

void RE::MessageBoxImpl::Data::Queue()
{
	using func_t = void (*)(void*, Data*);
	static REL::Relocation<void**> manager{ ID::MessageBox::Manager };
	static REL::Relocation<func_t> queue{ ID::MessageBox::Queue };
	queue(*manager, this);
}

void RE::ShowMessageBox(const char* a_title, const char* a_text, std::span<const char* const> a_buttons,
	const BSTSmartPointer<IMessageBoxCallback>& a_callback, bool a_allowBackOut)
{
	MessageBoxImpl::Info info{ a_title, a_text, a_callback };
	BSTArray<MessageBoxImpl::Button> buttons;
	buttons.reserve(static_cast<std::uint32_t>(a_buttons.size()));
	for (const char* text : a_buttons) {
		buttons.emplace_back(text);
	}
	MessageBoxImpl::Data data{ info, buttons, a_allowBackOut };
	data.Queue();
}
