#pragma once

#include "Core/Map/Position.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace sw::core
{
	class Unit;
	class Map;

	class IGameContext
	{
	public:
		virtual ~IGameContext() = default;

		virtual Unit* getUnit(uint32_t unitId) = 0;
		virtual std::vector<Unit*> getUnitsInRange(const Position& center, uint32_t range) = 0;
		virtual std::vector<Unit*> getAdjacentUnits(const Position& position) = 0;
		virtual Unit* getUnitAt(const Position& pos) = 0;
		virtual std::vector<Unit*> getAllUnits() = 0;

		virtual bool moveUnit(Unit* unit, const Position& newPos) = 0;
		virtual bool canMoveTo(const Position& pos) = 0;

		virtual void attackUnit(Unit* attacker, Unit* target, uint32_t damage) = 0;
		virtual void removeUnit(uint32_t unitId) = 0;

		virtual bool isValidPosition(const Position& pos) const = 0;
		virtual const Map* getMap() const = 0;

		virtual uint64_t getCurrentTick() const = 0;

		virtual void logUnitMoved(uint64_t tick, uint32_t unitId, uint32_t x, uint32_t y) = 0;
		virtual void logUnitAttacked(
			uint64_t tick, uint32_t attackerId, uint32_t targetId, uint32_t damage, uint32_t targetHp)
			= 0;
		virtual void logUnitDied(uint64_t tick, uint32_t unitId) = 0;
		virtual void logMarchStarted(
			uint64_t tick, uint32_t unitId, uint32_t fromX, uint32_t fromY, uint32_t toX, uint32_t toY)
			= 0;
		virtual void logMarchEnded(uint64_t tick, uint32_t unitId, uint32_t x, uint32_t y) = 0;
	};
}
