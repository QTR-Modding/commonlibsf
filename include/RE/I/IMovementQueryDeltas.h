#pragma once

#include "RE/I/IMovementInterface.h"
#include "RE/N/NiPoint.h"

namespace RE
{
	class IMovementQueryDeltas : public IMovementInterface
	{
	public:
		SF_RTTI(IMovementQueryDeltas);

		~IMovementQueryDeltas() override;  // 00

		virtual void Unk_01();  // 01
		virtual void Unk_02();  // 02
		virtual void Unk_03();  // 03
		virtual void Unk_04();  // 04
		virtual void Unk_05();  // 05
		virtual void Unk_06();  // 06
		virtual void Unk_07();  // 07
		virtual void Unk_08();  // 08
		virtual void Unk_09();  // 09
		virtual void Unk_0A();  // 0A
		virtual void Unk_0B();  // 0B
		virtual void Unk_0C();  // 0C
		virtual void Unk_0D();  // 0D
		virtual void Unk_0E();  // 0E

		// Copies integrated linear/angular increments when available in the current frame.
		// False leaves both outputs unchanged. This is not a pending-request completion test.
		virtual bool GetStoredDeltas(NiPoint3& a_linear, NiPoint3& a_angular) const = 0;  // 0F
	};
	static_assert(sizeof(IMovementQueryDeltas) == 0x8);
}
