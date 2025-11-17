#pragma once

#include "Core/UnitFactory/IUnitFactory.hpp"

#include <memory>
#include <string>
#include <vector>

namespace sw::core
{
	class Unit;
	class Position;
}

namespace sw::features
{
	class SwordsmanFactory : public core::IUnitFactory
	{
	public:
		std::shared_ptr<core::Unit> createUnit(
			uint32_t id, const core::Position& pos, const std::vector<uint32_t>& params) override;

		std::string getUnitType() const override;
		std::string getSpawnCommandName() const override;
		size_t getSpawnCommandParamCount() const override;
		void validateSpawnParams(const std::vector<uint32_t>& params) const override;
	};
}
