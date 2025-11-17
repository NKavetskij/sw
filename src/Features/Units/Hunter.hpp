#pragma once
#include "Core/GameContext/IGameContext.hpp"
#include "Core/Units/Unit.hpp"
#include "Core/Units/UnitStats/HealthStats.hpp"
#include "Core/Units/UnitStats/MeleeCombatStats.hpp"
#include "Core/Units/UnitStats/MovementStats.hpp"
#include "Core/Units/UnitStats/RangedCombatStats.hpp"

namespace sw::features
{
	class Hunter : public core::Unit
	{
	private:
		static constexpr uint32_t s_meleeRange = 1;
		static constexpr uint32_t s_rangedRangeMin = 2;
		static constexpr bool s_canMove = true;
		static constexpr uint32_t s_moveRange = 1;

	public:
		Hunter(
			uint32_t id,
			const core::Position& pos,
			uint32_t hp,
			uint32_t meleeAttack,
			uint32_t rangedAttack,
			uint32_t rangedRangeMax) :
				Unit(id, "Hunter", pos)
		{
			addStats<core::HealthStats, core::IHealthStats>(hp);
			addStats<core::MeleeCombatStats, core::IMeleeCombatStats>(meleeAttack, s_meleeRange);
			addStats<core::RangedCombatStats, core::IRangedCombatStats>(rangedAttack, s_rangedRangeMin, rangedRangeMax);
			addStats<core::MovementStats, core::IMovementStats>(s_canMove, s_moveRange);
		}

		void act(core::IGameContext& context) override;
	};
}
