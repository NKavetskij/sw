#pragma once

#include "IUnitStats.hpp"

namespace sw::core
{
	class RangedCombatStats : public IRangedCombatStats
	{
	private:
		uint32_t _rangedAttack;
		uint32_t _rangedRangeMin;
		uint32_t _rangedRangeMax;

	public:
		RangedCombatStats(uint32_t rangedAttack, uint32_t rangedRangeMin, uint32_t rangedRangeMax) :
				_rangedAttack(rangedAttack),
				_rangedRangeMin(rangedRangeMin),
				_rangedRangeMax(rangedRangeMax)
		{}

		std::string getName() const override
		{
			return "RangedCombat";
		}

		uint32_t getRangedAttack() const override
		{
			return _rangedAttack;
		}

		uint32_t getRangedRangeMin() const override
		{
			return _rangedRangeMin;
		}

		uint32_t getRangedRangeMax() const override
		{
			return _rangedRangeMax;
		}
	};
}
