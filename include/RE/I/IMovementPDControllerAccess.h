#pragma once

#include "RE/I/IMovementInterface.h"

namespace RE
{
	class IMovementPDControllerAccess : public IMovementInterface
	{
	public:
		SF_RTTI_VTABLE(IMovementPDControllerAccess);

		~IMovementPDControllerAccess() override;  // 00

		virtual void Unk_01();                                                   // 01
		virtual void SetParameters(float a_parameter1, float a_parameter2) = 0;  // 02
		virtual void ResetParameters() = 0;                                      // 03
	};
	static_assert(sizeof(IMovementPDControllerAccess) == 0x8);
}
