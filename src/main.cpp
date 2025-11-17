#include "Core/CommandProcessor/CommandProcessor.hpp"
#include "Core/GameLogic/GameLogic.hpp"
#include "Core/UnitFactory/UnitFactoryManager.hpp"
#include "Features/Units/UnitFactory/HunterFactory.hpp"
#include "Features/Units/UnitFactory/SwordsmanFactory.hpp"
#include "IO/System/TypeRegistry.hpp"

#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/March.hpp>
#include <IO/Commands/SpawnHunter.hpp>
#include <IO/Commands/SpawnSwordsman.hpp>
#include <IO/Events/MapCreated.hpp>
#include <IO/Events/MarchEnded.hpp>
#include <IO/Events/MarchStarted.hpp>
#include <IO/Events/UnitAttacked.hpp>
#include <IO/Events/UnitDied.hpp>
#include <IO/Events/UnitMoved.hpp>
#include <IO/Events/UnitSpawned.hpp>
#include <IO/System/CommandParser.hpp>
#include <IO/System/EventLog.hpp>
#include <IO/System/PrintDebug.hpp>
#include <fstream>
#include <iostream>
#include <memory>

int main(int argc, char** argv)
{
	using namespace sw;

	if (argc != 2)
	{
		throw std::runtime_error("Error: No file specified in command line argument");
	}

	std::ifstream file(argv[1]);
	if (!file)
	{
		throw std::runtime_error("Error: File not found - " + std::string(argv[1]));
	}

	sw::EventLog eventLog;

	core::UnitFactoryManager factoryManager;
	factoryManager.registerFactory(std::make_shared<features::SwordsmanFactory>());
	factoryManager.registerFactory(std::make_shared<features::HunterFactory>());

	core::GameLogic gameLogic(eventLog);
	core::CommandProcessor commandProcessor(gameLogic, factoryManager);

	std::cout << "Commands:\n";

	io::CommandParser parser;
	parser
		.add<io::CreateMap>(
			[&commandProcessor](auto command)
			{
				sw::printDebug(std::cout, command);
				commandProcessor.queueCreateMapCommand(command);
			})
		.add<io::SpawnSwordsman>(
			[&commandProcessor](auto command)
			{
				sw::printDebug(std::cout, command);
				std::vector<uint32_t> params = {command.unitId, command.x, command.y, command.hp, command.strength};
				commandProcessor.queueSpawnCommand("SPAWN_SWORDSMAN", params);
			})
		.add<io::SpawnHunter>(
			[&commandProcessor](auto command)
			{
				sw::printDebug(std::cout, command);
				std::vector<uint32_t> params = {
					command.unitId, command.x, command.y, command.hp, command.agility, command.strength, command.range};
				commandProcessor.queueSpawnCommand("SPAWN_HUNTER", params);
			})
		.add<io::March>(
			[&commandProcessor](auto command)
			{
				sw::printDebug(std::cout, command);
				commandProcessor.queueMarchCommand(command);
			});

	try
	{
		parser.parse(file);

		std::cout << "\n\nEvents:\n";

		gameLogic.runSimulation();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
}
