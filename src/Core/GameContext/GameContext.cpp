#include "Core/GameContext/GameContext.hpp"

#include "Core/Map/Map.hpp"
#include "Core/Units/Unit.hpp"
#include "IO/Events/MapCreated.hpp"
#include "IO/Events/MarchEnded.hpp"
#include "IO/Events/MarchStarted.hpp"
#include "IO/Events/UnitAttacked.hpp"
#include "IO/Events/UnitDied.hpp"
#include "IO/Events/UnitMoved.hpp"
#include "IO/Events/UnitSpawned.hpp"

namespace sw::core
{
	GameContext::GameContext(World& world, EventLog& eventLog) :
			_world(world),
			_eventLog(eventLog)
	{}

	Unit* GameContext::getUnit(uint32_t unitId)
	{
		return _world.getUnit(unitId);
	}

	std::vector<Unit*> GameContext::getUnitsInRange(const Position& center, uint32_t range)
	{
		auto units = _world.getUnitsInRange(center, range);
		units.erase(
			std::remove_if(units.begin(), units.end(), [](Unit* unit) { return !unit || !unit->isAlive(); }),
			units.end());
		return units;
	}

	std::vector<Unit*> GameContext::getAdjacentUnits(const Position& position)
	{
		auto units = _world.getAdjacentUnits(position);
		units.erase(
			std::remove_if(units.begin(), units.end(), [](Unit* unit) { return !unit || !unit->isAlive(); }),
			units.end());
		return units;
	}

	Unit* GameContext::getUnitAt(const Position& pos)
	{
		return _world.getUnitAt(pos);
	}

	std::vector<Unit*> GameContext::getAllUnits()
	{
		auto units = _world.getAllUnits();

		units.erase(
			std::remove_if(units.begin(), units.end(), [](Unit* unit) { return !unit || !unit->isAlive(); }),
			units.end());
		return units;
	}

	bool GameContext::moveUnit(Unit* unit, const Position& newPos)
	{
		if (_world.moveUnit(unit, newPos))
		{
			logUnitMoved(_world.getCurrentTick(), unit->getId(), newPos.x, newPos.y);
			return true;
		}
		return false;
	}

	bool GameContext::canMoveTo(const Position& pos)
	{
		return _world.isCellFree(pos) && _world.getMap()->isValidPosition(pos);
	}

	void GameContext::attackUnit(Unit* attacker, Unit* target, uint32_t damage)
	{
		if (!attacker || !target || !target->isAlive())
		{
			return;
		}

		uint32_t targetHpBefore = target->getHp();
		target->takeDamage(damage);
		uint32_t targetHpAfter = target->getHp();

		logUnitAttacked(_world.getCurrentTick(), attacker->getId(), target->getId(), damage, targetHpAfter);
	}

	void GameContext::removeUnit(uint32_t unitId)
	{
		logUnitDied(_world.getCurrentTick(), unitId);
		_world.removeUnit(unitId);
	}

	bool GameContext::isValidPosition(const Position& pos) const
	{
		return _world.getMap()->isValidPosition(pos);
	}

	const Map* GameContext::getMap() const
	{
		return _world.getMap();
	}

	uint64_t GameContext::getCurrentTick() const
	{
		return _world.getCurrentTick();
	}

	void GameContext::logUnitMoved(uint64_t tick, uint32_t unitId, uint32_t x, uint32_t y)
	{
		_eventLog.log(tick, io::UnitMoved{unitId, x, y});
	}

	void GameContext::logUnitAttacked(
		uint64_t tick, uint32_t attackerId, uint32_t targetId, uint32_t damage, uint32_t targetHp)
	{
		_eventLog.log(tick, io::UnitAttacked{attackerId, targetId, damage, targetHp});
	}

	void GameContext::logUnitDied(uint64_t tick, uint32_t unitId)
	{
		_eventLog.log(tick, io::UnitDied{unitId});
	}

	void GameContext::logMarchStarted(
		uint64_t tick, uint32_t unitId, uint32_t fromX, uint32_t fromY, uint32_t toX, uint32_t toY)
	{
		_eventLog.log(tick, io::MarchStarted{unitId, fromX, fromY, toX, toY});
	}

	void GameContext::logMarchEnded(uint64_t tick, uint32_t unitId, uint32_t x, uint32_t y)
	{
		_eventLog.log(tick, io::MarchEnded{unitId, x, y});
	}
}
