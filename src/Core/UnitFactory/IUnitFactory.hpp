#pragma once

#include "Core/Units/Unit.hpp"

#include <memory>
#include <string>
#include <vector>

namespace sw::core
{
	class IUnitFactory
	{
	public:
		virtual ~IUnitFactory() = default;
		virtual std::shared_ptr<Unit> createUnit(uint32_t id, const Position& pos, const std::vector<uint32_t>& params)
			= 0;
		virtual std::string getUnitType() const = 0;
		virtual std::string getSpawnCommandName() const = 0;
		virtual size_t getSpawnCommandParamCount() const = 0;
		virtual void validateSpawnParams(const std::vector<uint32_t>& params) const = 0;
	};
}
