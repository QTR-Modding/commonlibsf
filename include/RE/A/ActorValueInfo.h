#pragma once

#include "RE/A/ActorValues.h"
#include "RE/B/BGSEditorID.h"
#include "RE/T/TESForm.h"
#include "RE/T/TESFullName.h"

namespace RE
{
	class BGSResource;

	class ActorValueInfo :
		public TESForm,     // 00
		public TESFullName  // 38
	{
	public:
		SF_RTTI_VTABLE(ActorValueInfo);
		SF_FORMTYPE(AVIF);

		~ActorValueInfo() override;  // 00

		// members
		BGSEditorID      editorID;             // 048
		std::uint64_t    unk050;               // 058
		std::uint64_t    unk058;               // 060
		std::uint64_t    unk060;               // 068
		std::uint64_t    unk068;               // 070
		std::uint64_t    unk070;               // 078
		std::uint64_t    unk078;               // 080
		std::uint64_t    unk080;               // 088
		std::uint64_t    unk088;               // 090
		std::uint64_t    unk090;               // 098
		std::uint64_t    unk098;               // 0A0
		std::uint64_t    unk0A0;               // 0A8
		std::uint64_t    unk0A8;               // 0B0
		std::uint64_t    unk0B0;               // 0B8
		std::uint64_t    unk0B8;               // 0C0
		std::uint64_t    unk0C0;               // 0C8
		std::uint64_t    unk0C8;               // 0D0
		std::uint64_t    unk0D0;               // 0D8
		std::uint64_t    unk0D8;               // 0E0
		std::uint64_t    unk0E0;               // 0E8
		std::uint64_t    unk0E8;               // 0F0
		std::uint64_t    unk0F0;               // 0F8
		std::uint64_t    unk0F8;               // 100
		std::uint64_t    unk100;               // 108
		std::uint64_t    unk108;               // 110
		std::uint64_t    unk110;               // 118
		std::uint64_t    unk118;               // 120
		std::uint64_t    unk120;               // 128
		std::uint64_t    unk128;               // 130
		std::uint64_t    unk130;               // 138
		std::uint64_t    unk138;               // 140
		BGSResource*     resource;             // 148
		BSFixedString    abbreviation;         // 150
		std::uint32_t    index;                // 158
		std::uint32_t    flags;                // 15C
		ActorValue::Type avType;               // 160
		std::uint32_t    unk15C;               // 164
		std::uint32_t    fullCacheIndex;       // 168
		std::uint32_t    permanentCacheIndex;  // 16C
		std::uint64_t    unk168;               // 170
		float            defaultValue;         // 178
		std::uint32_t    sortIndex;            // 17C
	};
	static_assert(sizeof(ActorValueInfo) == 0x180);
	static_assert(offsetof(ActorValueInfo, editorID) == 0x48);
	static_assert(offsetof(ActorValueInfo, index) == 0x158);
	static_assert(offsetof(ActorValueInfo, flags) == 0x15C);
	static_assert(offsetof(ActorValueInfo, avType) == 0x160);
	static_assert(offsetof(ActorValueInfo, fullCacheIndex) == 0x168);
	static_assert(offsetof(ActorValueInfo, permanentCacheIndex) == 0x16C);
	static_assert(offsetof(ActorValueInfo, defaultValue) == 0x178);
	static_assert(offsetof(ActorValueInfo, sortIndex) == 0x17C);
}
