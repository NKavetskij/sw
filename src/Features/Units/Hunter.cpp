#include "Hunter.hpp"

namespace sw::features
{
	void Hunter::act(core::IGameContext& context)
	{
		if (!isAlive())
		{
			return;
		}

		if (getIsMarching())
		{
			if (!moveTowardsTarget(context))
			{
				auto blockedEnemy = findRandomAdjacentEnemy(context);
				if (blockedEnemy)
				{
					context.attackUnit(this, blockedEnemy, getMeleeAttack());
				}
			}
			return;
		}

		if (getHasMoveTarget())
		{
			if (!moveTowardsTarget(context))
			{
				auto blockedEnemy = findRandomAdjacentEnemy(context);
				if (blockedEnemy)
				{
					context.attackUnit(this, blockedEnemy, getMeleeAttack());
				}
			}
			return;
		}

		auto rangedTarget = findEnemyForRangedAttack(context);
		if (rangedTarget)
		{
			context.attackUnit(this, rangedTarget, getRangedAttack());
			return;
		}

		auto meleeTarget = findEnemyForMeleeAttack(context);
		if (meleeTarget)
		{
			context.attackUnit(this, meleeTarget, getMeleeAttack());
			return;
		}

		auto enemy = findRandomEnemy(context);
		if (enemy && canMove())
		{
			core::Position moveTarget = enemy->getPosition();
			double distance = getPosition().distanceTo(enemy->getPosition());

			if (distance > getRangedRangeMax())
			{
				int32_t dx = static_cast<int32_t>(enemy->getPosition().x) - static_cast<int32_t>(getPosition().x);
				int32_t dy = static_cast<int32_t>(enemy->getPosition().y) - static_cast<int32_t>(getPosition().y);

				double length = std::sqrt(dx * dx + dy * dy);
				if (length > 0)
				{
					double scale = getRangedRangeMax() / length;
					int32_t offsetX = static_cast<int32_t>(dx * scale);
					int32_t offsetY = static_cast<int32_t>(dy * scale);

					moveTarget = core::Position(
						static_cast<uint32_t>(static_cast<int32_t>(getPosition().x) + offsetX),
						static_cast<uint32_t>(static_cast<int32_t>(getPosition().y) + offsetY));
				}
			}

			setMoveTarget(moveTarget);
			moveTowardsTarget(context);
		}
	}
}
