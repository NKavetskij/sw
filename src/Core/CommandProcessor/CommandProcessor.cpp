#include "Core/CommandProcessor/CommandProcessor.hpp"

#include "Core/Map/Map.hpp"

#include <stdexcept>

namespace sw::core
{
	CommandProcessor::CommandProcessor(GameLogic& gameLogic, UnitFactoryManager& factoryManager) :
			_gameLogic(gameLogic),
			_factoryManager(factoryManager)
	{}

	void CommandProcessor::queueCreateMapCommand(const io::CreateMap& command)
	{
		_gameLogic.queueCommand(
			[this, command]()
			{
				if (command.width == 0 || command.height == 0)
				{
					throw std::invalid_argument("Map dimensions must be positive");
				}
				_gameLogic.initializeMap(command.width, command.height);
			});
	}

	void CommandProcessor::queueSpawnCommand(const std::string& commandName, const std::vector<uint32_t>& params)
	{
		_gameLogic.queueCommand([this, commandName, params]()
								{ _factoryManager.executeSpawnCommand(commandName, _gameLogic, params); });
	}

	void CommandProcessor::queueMarchCommand(const io::March& command)
	{
		_gameLogic.queueCommand(
			[this, command]()
			{
				validateUnitId(command.unitId);

				auto map = _gameLogic.getWorld().getMap();
				if (!map)
				{
					throw std::invalid_argument("Map not initialized");
				}

				Position targetPos(command.targetX, command.targetY);
				if (!map->isValidPosition(targetPos))
				{
					throw std::invalid_argument(
						"Position (" + std::to_string(command.targetX) + ", " + std::to_string(command.targetY)
						+ ") is outside map boundaries");
				}

				auto unit = _gameLogic.getWorld().getUnit(command.unitId);
				if (!unit)
				{
					throw std::invalid_argument("Unit with ID " + std::to_string(command.unitId) + " not found");
				}

				if (!unit->canMove())
				{
					throw std::invalid_argument("Unit with ID " + std::to_string(command.unitId) + " cannot move");
				}

				_gameLogic.processMarchCommand(command.unitId, command.targetX, command.targetY);
			});
	}

	void CommandProcessor::validateUnitId(uint32_t unitId)
	{
		if (unitId == 0)
		{
			throw std::invalid_argument("Unit ID must be positive");
		}
	}
}
