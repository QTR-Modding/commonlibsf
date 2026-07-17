#pragma once

#include "RE/B/BSInputEventUser.h"
#include "RE/E/Events.h"

namespace RE
{
	class PlayerControls::PlayerInputHandler :
		public BSInputEventUser  // 00
	{
	public:
		inline static constexpr auto RTTI = RE::RTTI::PlayerControls__PlayerInputHandler;

		~PlayerInputHandler() override;  // 00

		// add
		virtual void Unk_0A();  // 0A
		virtual bool Unk_0B();  // 0B
		virtual void Unk_0C();  // 0C
		virtual void Unk_0D();  // 0D
		virtual bool Unk_0E();  // 0E
		virtual void Unk_0F();  // 0F

		// members
		std::uint64_t unk40;     // 40
		bool          unk48;     // 48
		bool          unk49;     // 49
		bool          unk4A;     // 4A
		std::byte     pad4B[5];  // 4B
	};
	static_assert(sizeof(PlayerControls::PlayerInputHandler) == 0x50);

	class PlayerControls::JumpHandler :
		public PlayerInputHandler  // 00, VTABLE[0]
	{
	public:
		inline static constexpr auto RTTI = RE::RTTI::PlayerControls__JumpHandler;
		inline static constexpr auto VTABLE = RE::VTABLE::PlayerControls__JumpHandler;

		~JumpHandler() override;  // 00

		// override (BSInputEventUser)
		bool ShouldHandleEvent(const InputEvent* a_event) override;  // 01
		void OnButtonEvent(const ButtonEvent* a_event) override;     // 08

		// override (PlayerInputHandler)
		bool Unk_0B() override;  // 0B
		void Unk_0C() override;  // 0C
		void Unk_0D() override;  // 0D
		bool Unk_0E() override;  // 0E
		void Unk_0F() override;  // 0F

		// members
		bool      unk50;     // 50
		std::byte pad51[7];  // 51
	};
	static_assert(sizeof(PlayerControls::JumpHandler) == 0x58);
}
