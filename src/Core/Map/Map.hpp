#pragma once

#include "Position.hpp"

#include <vector>

namespace sw::core
{
	class Map
	{
	private:
		uint32_t _width;
		uint32_t _height;

	public:
		Map(uint32_t width, uint32_t height);

		uint32_t getWidth() const
		{
			return _width;
		}

		uint32_t getHeight() const
		{
			return _height;
		}

		bool isValidPosition(const Position& pos) const;
		std::vector<Position> getNeighborPositions(const Position& pos) const;
		std::vector<Position> getPositionsInRange(const Position& center, uint32_t range) const;
	};
}
