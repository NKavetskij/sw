#include "Core/UnitFactory/UnitFactoryManager.hpp"

#include "Core/GameLogic/GameLogic.hpp"

#include <stdexcept>

namespace sw::core
{
	void UnitFactoryManager::registerFactory(std::shared_ptr<IUnitFactory> factory)
	{
		std::string unitType = factory->getUnitType();
		std::string commandName = factory->getSpawnCommandName();

		_factories[unitType] = factory;
		_spawnCommands[commandName] = factory;
	}

	std::shared_ptr<Unit> UnitFactoryManager::createUnit(
		const std::string& unitType, uint32_t id, const Position& pos, const std::vector<uint32_t>& params)
	{
		auto it = _factories.find(unitType);
		if (it != _factories.end())
		{
			return it->second->createUnit(id, pos, params);
		}
		return nullptr;
	}

	void UnitFactoryManager::executeSpawnCommand(
		const std::string& commandName, GameLogic& gameLogic, const std::vector<uint32_t>& params)
	{
		auto it = _spawnCommands.find(commandName);
		if (it == _spawnCommands.end())
		{
			throw std::invalid_argument("Unknown spawn command: " + commandName);
		}

		auto& factory = it->second;

		factory->validateSpawnParams(params);

		if (params.size() < factory->getSpawnCommandParamCount())
		{
			throw std::invalid_argument("Not enough parameters for command " + commandName);
		}

		uint32_t unitId = params[0];
		uint32_t x = params[1];
		uint32_t y = params[2];

		if (unitId == 0)
		{
			throw std::invalid_argument("Unit ID must be positive");
		}

		Position pos(x, y);
		if (!gameLogic.getWorld().getMap() || !gameLogic.getWorld().getMap()->isValidPosition(pos))
		{
			throw std::invalid_argument("Invalid position");
		}

		if (!gameLogic.getWorld().isCellFree(pos))
		{
			throw std::invalid_argument("Position is already occupied");
		}

		std::vector<uint32_t> unitParams(params.begin() + 3, params.end());

		auto unit = factory->createUnit(unitId, pos, unitParams);

		if (!unit)
		{
			throw std::invalid_argument("Failed to create unit with command " + commandName);
		}

		gameLogic.addUnit(unit);
	}

	bool UnitFactoryManager::hasSpawnCommand(const std::string& commandName) const
	{
		return _spawnCommands.find(commandName) != _spawnCommands.end();
	}
}
