#include "SwordsmanFactory.hpp"

#include "Features/Units/Swordsman.hpp"

#include <stdexcept>

namespace sw::features
{
	std::shared_ptr<core::Unit> SwordsmanFactory::createUnit(
		uint32_t id, const core::Position& pos, const std::vector<uint32_t>& params)
	{
		if (params.size() < 2)
		{
			return nullptr;
		}
		uint32_t hp = params[0];
		uint32_t attack = params[1];
		return std::make_shared<Swordsman>(id, pos, hp, attack);
	}

	std::string SwordsmanFactory::getUnitType() const
	{
		return "Swordsman";
	}

	std::string SwordsmanFactory::getSpawnCommandName() const
	{
		return "SPAWN_SWORDSMAN";
	}

	size_t SwordsmanFactory::getSpawnCommandParamCount() const
	{
		return 5;
	}

	void SwordsmanFactory::validateSpawnParams(const std::vector<uint32_t>& params) const
	{
		if (params.size() < getSpawnCommandParamCount())
		{
			throw std::invalid_argument("SPAWN_SWORDSMAN requires 5 parameters: unitId, x, y, hp, strength");
		}

		if (params[0] == 0)
		{
			throw std::invalid_argument("Unit ID must be positive");
		}

		if (params[3] == 0)
		{
			throw std::invalid_argument("HP must be positive");
		}
	}
}
