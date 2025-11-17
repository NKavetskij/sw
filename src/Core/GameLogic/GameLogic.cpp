#include "GameLogic.hpp"

#include "Core/GameContext/GameContext.hpp"
#include "Core/Map/Map.hpp"
#include "Core/Units/Unit.hpp"
#include "IO/Events/MapCreated.hpp"
#include "IO/Events/MarchStarted.hpp"
#include "IO/Events/UnitSpawned.hpp"
#include "IO/System/EventLog.hpp"

#include <stdexcept>

namespace sw::core
{
	GameLogic::GameLogic(EventLog& eventLog) :
			_eventLog(eventLog)
	{
		_world = std::make_unique<World>();
	}

	void GameLogic::initializeMap(uint32_t width, uint32_t height)
	{
		_world->initializeMap(width, height);
		_eventLog.log(1, io::MapCreated{width, height});
	}

	void GameLogic::addUnit(std::shared_ptr<Unit> unit)
	{
		_world->addUnit(unit);
		_eventLog.log(1, io::UnitSpawned{unit->getId(), unit->getType(), unit->getPosition().x, unit->getPosition().y});
	}

	void GameLogic::processMarchCommand(uint32_t unitId, uint32_t targetX, uint32_t targetY)
	{
		auto unit = _world->getUnit(unitId);
		if (!unit)
		{
			throw std::invalid_argument("Unit with ID " + std::to_string(unitId) + " not found");
		}

		if (!unit->canMove())
		{
			throw std::invalid_argument("Unit with ID " + std::to_string(unitId) + " cannot move");
		}

		Position currentPos = unit->getPosition();
		Position targetPos(targetX, targetY);

		_eventLog.log(_world->getCurrentTick(), io::MarchStarted{unitId, currentPos.x, currentPos.y, targetX, targetY});

		unit->startMarch(targetPos);
	}

	void GameLogic::queueCommand(std::function<void()> command)
	{
		_commandQueue.push(command);
	}

	void GameLogic::processCommands()
	{
		while (!_commandQueue.empty())
		{
			auto command = _commandQueue.front();
			command();
			_commandQueue.pop();
		}
	}

	void GameLogic::executeUnitTurns()
	{
		GameContext context(*_world, _eventLog);
		auto units = _world->getUnitsInCreationOrder();

		for (auto unit : units)
		{
			if (unit && unit->canAct())
			{
				unit->act(context);
			}
		}

		_world->removeDeadUnits(_eventLog, _world->getCurrentTick());
		_world->incrementTick();
	}

	void GameLogic::runTurn()
	{
		executeUnitTurns();
	}

	void GameLogic::runSimulation()
	{
		processCommands();
		_world->incrementTick();

		while (!hasGameEnded())
		{
			executeUnitTurns();
		}
	}

	bool GameLogic::hasGameEnded() const
	{
		int aliveCount = 0;
		for (auto unit : _world->getAllUnits())
		{
			if (unit->isAlive() && unit->canAct())
			{
				aliveCount++;
				if (aliveCount > 1)
				{
					return false;
				}
			}
		}
		return true;
	}
}
