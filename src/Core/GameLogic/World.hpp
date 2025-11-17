#pragma once

#include "Core/Map/Map.hpp"
#include "Core/Units/Unit.hpp"
#include "IO/System/EventLog.hpp"

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace sw::core
{
	class World
	{
	private:
		std::unique_ptr<Map> _map;
		std::vector<std::shared_ptr<Unit>> _units;
		std::unordered_map<Position, Unit*> _unitPositions;
		uint64_t _currentTick = 1;

	public:
		void initializeMap(uint32_t width, uint32_t height);
		void addUnit(std::shared_ptr<Unit> unit);
		void removeUnit(uint32_t unitId);
		void removeDeadUnits(sw::EventLog& eventLog, uint64_t currentTick);

		void incrementTick()
		{
			_currentTick++;
		}

		uint64_t getCurrentTick() const
		{
			return _currentTick;
		}

		Unit* getUnit(uint32_t unitId);
		const Map* getMap() const;

		bool isCellFree(const Position& pos) const;
		bool moveUnit(Unit* unit, const Position& newPos);
		Unit* getUnitAt(const Position& pos) const;

		std::vector<Unit*> getUnitsInRange(const Position& center, uint32_t range) const;
		std::vector<Unit*> getAdjacentUnits(const Position& position) const;
		std::vector<Unit*> getUnitsInCreationOrder() const;
		std::vector<Unit*> getAllUnits() const;

	private:
		void removeUnitFromPosition(Unit* unit);
	};
}
