#pragma once

#include "Core/GameContext/IGameContext.hpp"
#include "Core/Map/Position.hpp"
#include "IO/System/TypeRegistry.hpp"
#include "UnitStats/IUnitStats.hpp"

#include <memory>
#include <string>

namespace sw::core
{
	class Unit
	{
	private:
		uint32_t _id;
		std::string _type;
		Position _position;
		TypeRegistry _stats;

		Position _moveTarget;
		bool _hasMoveTarget = false;
		bool _isMarching = false;

	public:
		Unit(uint32_t id, const std::string& type, const Position& pos);

		virtual ~Unit() = default;

		uint32_t getId() const;
		std::string getType() const;
		Position getPosition() const;
		void setPosition(const Position& newPos);

		bool isAlive() const;
		uint32_t getHp() const;
		virtual bool canMove() const;
		virtual bool canAct() const;
		void takeDamage(uint32_t damage);

		virtual void act(core::IGameContext& context) = 0;

		virtual void startMarch(const Position& target);
		virtual bool hasMoveTarget() const;
		virtual Position getMoveTarget() const;
		virtual void clearMoveTarget();
		virtual bool isMarching() const;

		void setMoveTarget(const Position& target);

		Position calculateNextStep(const Position& target) const;
		bool moveTowardsTarget(core::IGameContext& context);

		Unit* findRandomEnemy(core::IGameContext& context);
		Unit* findRandomAdjacentEnemy(core::IGameContext& context);
		Unit* findEnemyInRange(core::IGameContext& context, uint32_t minRange, uint32_t maxRange);

		uint32_t getMeleeAttack() const;
		uint32_t getMeleeRange() const;
		bool canAttackMelee() const;
		Unit* findEnemyForMeleeAttack(core::IGameContext& context);
		bool isInMeleeRange(const Position& targetPos) const;

		uint32_t getRangedAttack() const;
		uint32_t getRangedRangeMin() const;
		uint32_t getRangedRangeMax() const;
		bool canAttackRanged() const;
		Unit* findEnemyForRangedAttack(core::IGameContext& context);
		bool isInRangedRange(const Position& targetPos) const;

		uint32_t getMoveRange() const;

		bool getHasMoveTarget() const;
		bool getIsMarching() const;
		const Position& getMoveTargetPos() const;

		template <typename TStats, typename... TInterfaces, typename... Args>
		std::shared_ptr<TStats> addStats(Args&&... args)
		{
			return _stats.emplace<TStats, TInterfaces...>(std::forward<Args>(args)...);
		}

		template <typename TStats>
		std::shared_ptr<TStats> getStats() const
		{
			return _stats.get<TStats>();
		}

		template <typename TInterface>
		bool hasStats() const
		{
			return _stats.get<TInterface>() != nullptr;
		}
	};
}
