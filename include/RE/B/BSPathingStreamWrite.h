#pragma once

#include "RE/B/BSFixedString.h"

namespace RE
{
	// Engine-owned string/scalar prefix; concrete streams have additional virtuals.
	class BSPathingStreamWrite
	{
	public:
		SF_RTTI(BSPathingStreamWrite);

		// Requires a non-null interned string.
		virtual bool WriteString(const BSFixedString& a_value) = 0;  // 00

		// Scalar I/O prefix used by native movement serialization. The stream owns its cursor.
		virtual bool WriteFloat(float a_value) = 0;           // 01
		virtual bool WriteUInt64(std::uint64_t a_value) = 0;  // 02
		virtual bool WriteInt64(std::int64_t a_value) = 0;    // 03
		virtual bool WriteUInt32(std::uint32_t a_value) = 0;  // 04
		virtual bool WriteInt32(std::int32_t a_value) = 0;    // 05
		virtual bool WriteUInt16(std::uint16_t a_value) = 0;  // 06
		virtual bool WriteInt16(std::int16_t a_value) = 0;    // 07
		virtual bool WriteUInt8(std::uint8_t a_value) = 0;    // 08
		virtual bool WriteInt8(std::int8_t a_value) = 0;      // 09
		virtual bool WriteChar(char a_value) = 0;             // 0A
		virtual bool WriteBool(bool a_value) = 0;             // 0B

	protected:
		~BSPathingStreamWrite() = default;
	};
	static_assert(sizeof(BSPathingStreamWrite) == 0x8);
}
