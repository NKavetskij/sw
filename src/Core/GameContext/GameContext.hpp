#pragma once

#include "Core/GameLogic/World.hpp"
#include "IGameContext.hpp"
#include "IO/System/EventLog.hpp"

namespace sw::core
{
	class GameContext : public IGameContext
	{
	private:
		World& _world;
		EventLog& _eventLog;

	public:
		GameContext(World& world, EventLog& eventLog);

		Unit* getUnit(uint32_t unitId) override;
		std::vector<Unit*> getUnitsInRange(const Position& center, uint32_t range) override;
		std::vector<Unit*> getAdjacentUnits(const Position& position) override;
		Unit* getUnitAt(const Position& pos) override;
		std::vector<Unit*> getAllUnits() override;

		bool moveUnit(Unit* unit, const Position& newPos) override;
		bool canMoveTo(const Position& pos) override;

		void attackUnit(Unit* attacker, Unit* target, uint32_t damage) override;
		void removeUnit(uint32_t unitId) override;

		bool isValidPosition(const Position& pos) const override;
		const Map* getMap() const override;

		uint64_t getCurrentTick() const override;

		void logUnitMoved(uint64_t tick, uint32_t unitId, uint32_t x, uint32_t y) override;
		void logUnitAttacked(
			uint64_t tick, uint32_t attackerId, uint32_t targetId, uint32_t damage, uint32_t targetHp) override;
		void logUnitDied(uint64_t tick, uint32_t unitId) override;
		void logMarchStarted(
			uint64_t tick, uint32_t unitId, uint32_t fromX, uint32_t fromY, uint32_t toX, uint32_t toY) override;
		void logMarchEnded(uint64_t tick, uint32_t unitId, uint32_t x, uint32_t y) override;
	};
}
