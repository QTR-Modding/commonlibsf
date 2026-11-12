#pragma once

namespace RE
{
	class BlueprintCellReadScope
	{
	public:
		explicit BlueprintCellReadScope(std::uintptr_t a_database);
		~BlueprintCellReadScope();
		BlueprintCellReadScope(const BlueprintCellReadScope&) = delete;
		BlueprintCellReadScope& operator=(const BlueprintCellReadScope&) = delete;

	private:
		std::byte*    _owner;
		std::int32_t* _readCount;
	};
}
