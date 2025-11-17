#pragma once

#include "IUnitStats.hpp"

namespace sw::core
{
	class MovementStats : public IMovementStats
	{
	private:
		bool _canMove;
		uint32_t _moveRange;
		uint32_t _speed;

	public:
		MovementStats(bool canMove, uint32_t moveRange = 1, uint32_t speed = 1) :
				_canMove(canMove),
				_moveRange(moveRange),
				_speed(speed)
		{}

		std::string getName() const override
		{
			return "Movement";
		}

		bool canMove() const override
		{
			return _canMove;
		}

		uint32_t getMoveRange() const override
		{
			return _moveRange;
		}
	};
}
