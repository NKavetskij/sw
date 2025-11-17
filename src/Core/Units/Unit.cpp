#include "Unit.hpp"

#include "UnitStats/HealthStats.hpp"
#include "UnitStats/MeleeCombatStats.hpp"
#include "UnitStats/MovementStats.hpp"
#include "UnitStats/RangedCombatStats.hpp"

namespace sw::core
{
	Unit::Unit(uint32_t id, const std::string& type, const Position& pos) :
			_id(id),
			_type(type),
			_position(pos)
	{}

	uint32_t Unit::getId() const
	{
		return _id;
	}

	std::string Unit::getType() const
	{
		return _type;
	}

	Position Unit::getPosition() const
	{
		return _position;
	}

	void Unit::setPosition(const Position& newPos)
	{
		_position = newPos;
	}

	bool Unit::isAlive() const
	{
		if (auto health = getStats<IHealthStats>())
		{
			return health->getHp() > 0;
		}
		return true;
	}

	uint32_t Unit::getHp() const
	{
		if (auto health = getStats<IHealthStats>())
		{
			return health->getHp();
		}
		return 0;
	}

	bool Unit::canMove() const
	{
		if (auto movement = getStats<IMovementStats>())
		{
			return movement->canMove();
		}
		return false;
	}

	bool Unit::canAct() const
	{
		return isAlive();
	}

	void Unit::takeDamage(uint32_t damage)
	{
		if (auto health = getStats<IHealthStats>())
		{
			health->takeDamage(damage);
		}
	}

	void Unit::startMarch(const Position& target)
	{
		_moveTarget = target;
		_hasMoveTarget = true;
		_isMarching = true;
	}

	bool Unit::hasMoveTarget() const
	{
		return _hasMoveTarget;
	}

	Position Unit::getMoveTarget() const
	{
		return _moveTarget;
	}

	void Unit::clearMoveTarget()
	{
		_hasMoveTarget = false;
		_isMarching = false;
	}

	bool Unit::isMarching() const
	{
		return _isMarching;
	}

	void Unit::setMoveTarget(const Position& target)
	{
		_moveTarget = target;
		_hasMoveTarget = true;
	}

	bool Unit::getHasMoveTarget() const
	{
		return _hasMoveTarget;
	}

	bool Unit::getIsMarching() const
	{
		return _isMarching;
	}

	const Position& Unit::getMoveTargetPos() const
	{
		return _moveTarget;
	}

	Position Unit::calculateNextStep(const Position& target) const
	{
		if (_position == target)
		{
			return _position;
		}

		int32_t dx = static_cast<int32_t>(target.x) - static_cast<int32_t>(_position.x);
		int32_t dy = static_cast<int32_t>(target.y) - static_cast<int32_t>(_position.y);

		int32_t stepX = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;
		int32_t stepY = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;

		return Position(
			static_cast<uint32_t>(static_cast<int32_t>(_position.x) + stepX),
			static_cast<uint32_t>(static_cast<int32_t>(_position.y) + stepY));
	}

	bool Unit::moveTowardsTarget(core::IGameContext& context)
	{
		if (!_hasMoveTarget || !canMove())
		{
			return false;
		}

		if (getPosition() == _moveTarget)
		{
			if (_isMarching)
			{
				context.logMarchEnded(context.getCurrentTick(), getId(), _moveTarget.x, _moveTarget.y);
			}
			clearMoveTarget();
			return true;
		}

		auto nextPos = calculateNextStep(_moveTarget);

		if (context.canMoveTo(nextPos))
		{
			if (context.moveUnit(this, nextPos))
			{
				setPosition(nextPos);

				if (getPosition() == _moveTarget)
				{
					if (_isMarching)
					{
						context.logMarchEnded(context.getCurrentTick(), getId(), _moveTarget.x, _moveTarget.y);
					}
					clearMoveTarget();
				}
				return true;
			}
		}

		return false;
	}

	Unit* Unit::findRandomEnemy(core::IGameContext& context)
	{
		auto allUnits = context.getAllUnits();
		std::vector<Unit*> enemies;

		for (auto unit : allUnits)
		{
			if (unit && unit != this && unit->isAlive())
			{
				enemies.push_back(unit);
			}
		}

		if (enemies.empty())
		{
			return nullptr;
		}

		Unit* closestEnemy = nullptr;
		double minDistance = std::numeric_limits<double>::max();

		for (auto enemy : enemies)
		{
			double distance = _position.distanceTo(enemy->getPosition());
			if (distance < minDistance)
			{
				minDistance = distance;
				closestEnemy = enemy;
			}
		}

		return closestEnemy;
	}

	Unit* Unit::findRandomAdjacentEnemy(core::IGameContext& context)
	{
		auto adjacentUnits = context.getAdjacentUnits(getPosition());
		for (auto unit : adjacentUnits)
		{
			if (unit && unit != this && unit->isAlive())
			{
				return unit;
			}
		}
		return nullptr;
	}

	Unit* Unit::findEnemyInRange(core::IGameContext& context, uint32_t minRange, uint32_t maxRange)
	{
		auto allUnits = context.getAllUnits();
		for (auto unit : allUnits)
		{
			if (unit && unit->isAlive() && unit->getId() != _id)
			{
				double distance = _position.distanceTo(unit->getPosition());
				if (distance >= minRange && distance <= maxRange)
				{
					return unit;
				}
			}
		}
		return nullptr;
	}

	uint32_t Unit::getMeleeAttack() const
	{
		if (auto melee = getStats<IMeleeCombatStats>())
		{
			return melee->getMeleeAttack();
		}
		return 0;
	}

	uint32_t Unit::getMeleeRange() const
	{
		if (auto melee = getStats<IMeleeCombatStats>())
		{
			return melee->getMeleeRange();
		}
		return 0;
	}

	bool Unit::canAttackMelee() const
	{
		return getMeleeAttack() > 0 && getMeleeRange() > 0;
	}

	Unit* Unit::findEnemyForMeleeAttack(core::IGameContext& context)
	{
		auto allUnits = context.getAllUnits();
		for (auto unit : allUnits)
		{
			if (unit && unit->isAlive() && unit->getId() != _id && isInMeleeRange(unit->getPosition()))
			{
				return unit;
			}
		}
		return nullptr;
	}

	bool Unit::isInMeleeRange(const Position& targetPos) const
	{
		double distance = _position.distanceTo(targetPos);
		return distance <= getMeleeRange();
	}

	uint32_t Unit::getRangedAttack() const
	{
		if (auto ranged = getStats<IRangedCombatStats>())
		{
			return ranged->getRangedAttack();
		}
		return 0;
	}

	uint32_t Unit::getRangedRangeMin() const
	{
		if (auto ranged = getStats<IRangedCombatStats>())
		{
			return ranged->getRangedRangeMin();
		}
		return 0;
	}

	uint32_t Unit::getRangedRangeMax() const
	{
		if (auto ranged = getStats<IRangedCombatStats>())
		{
			return ranged->getRangedRangeMax();
		}
		return 0;
	}

	bool Unit::canAttackRanged() const
	{
		return getRangedAttack() > 0 && getRangedRangeMax() > 0;
	}

	Unit* Unit::findEnemyForRangedAttack(core::IGameContext& context)
	{
		auto adjacentUnits = context.getAdjacentUnits(getPosition());
		for (auto unit : adjacentUnits)
		{
			if (unit && unit->isAlive() && unit->getId() != _id)
			{
				return nullptr;
			}
		}

		auto allUnits = context.getAllUnits();
		for (auto unit : allUnits)
		{
			if (unit && unit->isAlive() && unit->getId() != _id && isInRangedRange(unit->getPosition()))
			{
				return unit;
			}
		}
		return nullptr;
	}

	bool Unit::isInRangedRange(const Position& targetPos) const
	{
		double distance = _position.distanceTo(targetPos);
		return distance >= getRangedRangeMin() && distance <= getRangedRangeMax();
	}

	uint32_t Unit::getMoveRange() const
	{
		if (auto movement = getStats<IMovementStats>())
		{
			return movement->getMoveRange();
		}
		return 1;
	}
}
