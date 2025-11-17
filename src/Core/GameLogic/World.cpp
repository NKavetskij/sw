#include "World.hpp"

#include "Core/GameContext/GameContext.hpp"
#include "IO/Events/UnitDied.hpp"

namespace sw::core
{
	void World::initializeMap(uint32_t width, uint32_t height)
	{
		_map = std::make_unique<Map>(width, height);
	}

	void World::addUnit(std::shared_ptr<Unit> unit)
	{
		Position pos = unit->getPosition();
		if (!isCellFree(pos))
		{
			throw std::runtime_error(
				"Position already occupied: " + std::to_string(pos.x) + "," + std::to_string(pos.y));
		}

		_units.push_back(unit);
		_unitPositions[pos] = unit.get();
	}

	void World::removeUnit(uint32_t unitId)
	{
		for (auto it = _units.begin(); it != _units.end(); ++it)
		{
			if ((*it)->getId() == unitId)
			{
				removeUnitFromPosition(it->get());
				_units.erase(it);
				break;
			}
		}
	}

	void World::removeDeadUnits(sw::EventLog& eventLog, uint64_t currentTick)
	{
		for (auto it = _units.begin(); it != _units.end();)
		{
			if (!(*it)->isAlive())
			{
				eventLog.log(currentTick, io::UnitDied{(*it)->getId()});
				removeUnitFromPosition(it->get());
				it = _units.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	Unit* World::getUnit(uint32_t unitId)
	{
		for (auto& unit : _units)
		{
			if (unit->getId() == unitId && unit->isAlive())
			{
				return unit.get();
			}
		}
		return nullptr;
	}

	const Map* World::getMap() const
	{
		return _map.get();
	}

	bool World::isCellFree(const Position& pos) const
	{
		return _map->isValidPosition(pos) && _unitPositions.find(pos) == _unitPositions.end();
	}

	bool World::moveUnit(Unit* unit, const Position& newPos)
	{
		if (!isCellFree(newPos))
		{
			return false;
		}

		removeUnitFromPosition(unit);
		_unitPositions[newPos] = unit;
		return true;
	}

	void World::removeUnitFromPosition(Unit* unit)
	{
		for (auto it = _unitPositions.begin(); it != _unitPositions.end();)
		{
			if (it->second == unit)
			{
				it = _unitPositions.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	Unit* World::getUnitAt(const Position& pos) const
	{
		auto it = _unitPositions.find(pos);
		return it != _unitPositions.end() ? it->second : nullptr;
	}

	std::vector<Unit*> World::getUnitsInRange(const Position& center, uint32_t range) const
	{
		std::vector<Unit*> units;
		for (const auto& unit : _units)
		{
			if (unit->isAlive() && center.distanceTo(unit->getPosition()) <= range)
			{
				units.push_back(unit.get());
			}
		}
		return units;
	}

	std::vector<Unit*> World::getAdjacentUnits(const Position& position) const
	{
		std::vector<Unit*> units;
		auto adjacentPositions = _map->getNeighborPositions(position);
		for (const auto& pos : adjacentPositions)
		{
			if (auto unit = getUnitAt(pos))
			{
				units.push_back(unit);
			}
		}
		return units;
	}

	std::vector<Unit*> World::getUnitsInCreationOrder() const
	{
		std::vector<Unit*> units;
		for (const auto& unit : _units)
		{
			if (unit->isAlive())
			{
				units.push_back(unit.get());
			}
		}
		return units;
	}

	std::vector<Unit*> World::getAllUnits() const
	{
		std::vector<Unit*> units;
		for (const auto& unit : _units)
		{
			units.push_back(unit.get());
		}
		return units;
	}
}
