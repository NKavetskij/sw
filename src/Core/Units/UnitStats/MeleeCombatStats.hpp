#pragma once

#include "IUnitStats.hpp"

namespace sw::core
{
	class MeleeCombatStats : public IMeleeCombatStats
	{
	private:
		uint32_t _meleeAttack;
		uint32_t _meleeRange;

	public:
		MeleeCombatStats(uint32_t meleeAttack, uint32_t meleeRange) :
				_meleeAttack(meleeAttack),
				_meleeRange(meleeRange)
		{}

		std::string getName() const override
		{
			return "MeleeCombat";
		}

		uint32_t getMeleeAttack() const override
		{
			return _meleeAttack;
		}

		uint32_t getMeleeRange() const override
		{
			return _meleeRange;
		}
	};
}
