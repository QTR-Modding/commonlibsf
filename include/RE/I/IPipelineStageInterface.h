#pragma once

namespace RE
{
	struct IPipelineStageInterface
	{
	public:
		SF_RTTI(IPipelineStageInterface);

		virtual ~IPipelineStageInterface();  // 00

		virtual void Unk_01();  // 01
		virtual void Unk_02();  // 02
		virtual void Unk_03();  // 03
	};
	static_assert(sizeof(IPipelineStageInterface) == 0x8);
}
