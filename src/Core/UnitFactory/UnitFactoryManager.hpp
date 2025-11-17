#pragma once

#include "IUnitFactory.hpp"

#include <memory>
#include <unordered_map>

namespace sw::core
{
	class GameLogic;

	class UnitFactoryManager
	{
	private:
		std::unordered_map<std::string, std::shared_ptr<IUnitFactory>> _factories;
		std::unordered_map<std::string, std::shared_ptr<IUnitFactory>> _spawnCommands;

	public:
		void registerFactory(std::shared_ptr<IUnitFactory> factory);

		std::shared_ptr<Unit> createUnit(
			const std::string& unitType, uint32_t id, const Position& pos, const std::vector<uint32_t>& params);
		void executeSpawnCommand(
			const std::string& commandName, GameLogic& gameLogic, const std::vector<uint32_t>& params);

		bool hasSpawnCommand(const std::string& commandName) const;

		const IUnitFactory* getFactoryForCommand(const std::string& commandName) const;
	};
}
