#pragma once

#include "IUnitStats.hpp"

namespace sw::core
{
	class HealthStats : public IHealthStats
	{
	private:
		uint32_t _hp;
		uint32_t _maxHp;

	public:
		HealthStats(uint32_t hp) :
				_hp(hp),
				_maxHp(hp)
		{}

		std::string getName() const override
		{
			return "Health";
		}

		uint32_t getHp() const override
		{
			return _hp;
		}

		uint32_t getMaxHp() const override
		{
			return _maxHp;
		}

		void takeDamage(uint32_t damage) override
		{
			if (_hp == 0)
			{
				return;
			}

			if (damage >= _hp)
			{
				_hp = 0;
			}
			else
			{
				_hp -= damage;
			}
		}
	};
}
