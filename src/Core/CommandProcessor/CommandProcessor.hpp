#pragma once

#include "Core/GameLogic/GameLogic.hpp"
#include "Core/UnitFactory/UnitFactoryManager.hpp"
#include "IO/Commands/CreateMap.hpp"
#include "IO/Commands/March.hpp"

namespace sw::core
{
	class CommandProcessor
	{
	private:
		GameLogic& _gameLogic;
		UnitFactoryManager& _factoryManager;

	public:
		explicit CommandProcessor(GameLogic& gameLogic, UnitFactoryManager& factoryManager);

		void queueCreateMapCommand(const io::CreateMap& command);
		void queueSpawnCommand(const std::string& commandName, const std::vector<uint32_t>& params);
		void queueMarchCommand(const io::March& command);

	private:
		void validateUnitId(uint32_t unitId);
	};
}
