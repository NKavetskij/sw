#pragma once

#include "IO/System/EventLog.hpp"
#include "World.hpp"

#include <functional>
#include <queue>

namespace sw::core
{
	class GameLogic
	{
	private:
		std::unique_ptr<World> _world;
		EventLog& _eventLog;
		std::queue<std::function<void()>> _commandQueue;

		void processCommands();
		void executeUnitTurns();

	public:
		explicit GameLogic(EventLog& eventLog);

		void initializeMap(uint32_t width, uint32_t height);
		void addUnit(std::shared_ptr<Unit> unit);
		void processMarchCommand(uint32_t unitId, uint32_t targetX, uint32_t targetY);

		void queueCommand(std::function<void()> command);
		void runTurn();
		void runSimulation();

		World& getWorld()
		{
			return *_world;
		}

		EventLog& getEventLog()
		{
			return _eventLog;
		}

		bool hasGameEnded() const;
	};
}
