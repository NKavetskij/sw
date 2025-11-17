#include "Swordsman.hpp"

namespace sw::features
{
	void Swordsman::act(core::IGameContext& context)
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

		auto meleeTarget = findEnemyForMeleeAttack(context);
		if (meleeTarget)
		{
			context.attackUnit(this, meleeTarget, getMeleeAttack());
			return;
		}

		auto enemy = findRandomEnemy(context);
		if (enemy && canMove())
		{
			setMoveTarget(enemy->getPosition());
			moveTowardsTarget(context);
		}
	}
}
