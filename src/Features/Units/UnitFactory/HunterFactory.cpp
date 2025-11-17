#include "HunterFactory.hpp"

#include "Features/Units/Hunter.hpp"

#include <stdexcept>

namespace sw::features
{
	std::shared_ptr<core::Unit> HunterFactory::createUnit(
		uint32_t id, const core::Position& pos, const std::vector<uint32_t>& params)
	{
		if (params.size() < 4)
		{
			return nullptr;
		}
		uint32_t hp = params[0];
		uint32_t agility = params[1];
		uint32_t attack = params[2];
		uint32_t rangedRangeMax = params[3];
		return std::make_shared<Hunter>(id, pos, hp, attack, agility, rangedRangeMax);
	}

	std::string HunterFactory::getUnitType() const
	{
		return "Hunter";
	}

	std::string HunterFactory::getSpawnCommandName() const
	{
		return "SPAWN_HUNTER";
	}

	size_t HunterFactory::getSpawnCommandParamCount() const
	{
		return 7;
	}

	void HunterFactory::validateSpawnParams(const std::vector<uint32_t>& params) const
	{
		if (params.size() < getSpawnCommandParamCount())
		{
			throw std::invalid_argument(
				"SPAWN_HUNTER requires 7 parameters: unitId, x, y, hp, agility, strength, range");
		}

		if (params[0] == 0)
		{
			throw std::invalid_argument("Unit ID must be positive");
		}

		if (params[3] == 0)
		{
			throw std::invalid_argument("HP must be positive");
		}

		if (params[6] < 2)
		{
			throw std::invalid_argument("Range must be >= 2");
		}
	}
}
