#pragma once

#include <cstdint>
#include <string>

namespace sw::core
{
	class IUnitStats
	{
	public:
		virtual ~IUnitStats() = default;
		virtual std::string getName() const = 0;
	};

	class IHealthStats : public IUnitStats
	{
	public:
		virtual uint32_t getHp() const = 0;
		virtual uint32_t getMaxHp() const = 0;
		virtual void takeDamage(uint32_t damage) = 0;
		//virtual void heal(uint32_t amount) = 0; //TODO: healing mechanism
	};

	class IMeleeCombatStats : public IUnitStats
	{
	public:
		virtual uint32_t getMeleeAttack() const = 0;
		virtual uint32_t getMeleeRange() const = 0;
	};

	class IRangedCombatStats : public IUnitStats
	{
	public:
		virtual uint32_t getRangedAttack() const = 0;
		virtual uint32_t getRangedRangeMin() const = 0;
		virtual uint32_t getRangedRangeMax() const = 0;
	};

	class IMovementStats : public IUnitStats
	{
	public:
		virtual bool canMove() const = 0;
		virtual uint32_t getMoveRange() const = 0;	//TODO: move range mechanism (1 cell for now)
		//virtual uint32_t getSpeed() const = 0;     //TODO: sets the turn order priority
	};

	// class ISpecialStats : public IUnitStats
	// {
	// public:
	//     virtual bool isFlying() const = 0;    //TODO: flying mechanism
	//     virtual bool isHealer() const = 0;    //TODO: healing mechanism
	//     virtual bool isStealth() const = 0;   //TODO: stealth mechanism, can't be attacked (for mine for example)
	//     virtual bool isExplosive() const = 0; //TODO: explosion mechanism
	// };

}
