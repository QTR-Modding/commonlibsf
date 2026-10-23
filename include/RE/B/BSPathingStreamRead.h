#pragma once

#include "RE/B/BSFixedString.h"

namespace RE
{
	// Engine-owned string/scalar prefix; concrete streams have additional virtuals.
	class BSPathingStreamRead
	{
	public:
		SF_RTTI(BSPathingStreamRead);

		// A failed string read may already have consumed its length prefix.
		virtual bool ReadString(BSFixedString& a_value) = 0;  // 00

		// Scalar I/O prefix used by native movement serialization. The stream owns its cursor.
		virtual bool ReadFloat(float& a_value) = 0;           // 01
		virtual bool ReadUInt64(std::uint64_t& a_value) = 0;  // 02
		virtual bool ReadInt64(std::int64_t& a_value) = 0;    // 03
		virtual bool ReadUInt32(std::uint32_t& a_value) = 0;  // 04
		virtual bool ReadInt32(std::int32_t& a_value) = 0;    // 05
		virtual bool ReadUInt16(std::uint16_t& a_value) = 0;  // 06
		virtual bool ReadInt16(std::int16_t& a_value) = 0;    // 07
		virtual bool ReadUInt8(std::uint8_t& a_value) = 0;    // 08
		virtual bool ReadInt8(std::int8_t& a_value) = 0;      // 09
		virtual bool ReadChar(char& a_value) = 0;             // 0A
		virtual bool ReadBool(bool& a_value) = 0;             // 0B

	protected:
		~BSPathingStreamRead() = default;
	};
	static_assert(sizeof(BSPathingStreamRead) == 0x8);
}
