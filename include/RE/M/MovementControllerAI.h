#pragma once

namespace RE
{
	class MovementControllerAI
	{
	public:
		SF_RTTI_VTABLE(MovementControllerAI);

		virtual ~MovementControllerAI();  // 00

		// members
		std::byte    unk08[0x55];     // 08
		bool         controlsDriven;  // 5D
		std::byte    unk5E[0x12];     // 5E
		std::uint8_t movementMode;    // 70 - 1 = animation-driven, 2 = motion-driven
		std::byte    unk71[0x07];     // 71
	};
	static_assert(offsetof(MovementControllerAI, controlsDriven) == 0x5D);
	static_assert(offsetof(MovementControllerAI, movementMode) == 0x70);
	static_assert(sizeof(MovementControllerAI) == 0x78);
}
