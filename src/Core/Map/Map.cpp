#include "Map.hpp"

namespace sw::core
{
	Map::Map(uint32_t width, uint32_t height) :
			_width(width),
			_height(height)
	{}

	bool Map::isValidPosition(const Position& pos) const
	{
		return pos.x < _width && pos.y < _height;
	}

	std::vector<Position> Map::getNeighborPositions(const Position& pos) const
	{
		std::vector<Position> positions;
		for (int dx = -1; dx <= 1; ++dx)
		{
			for (int dy = -1; dy <= 1; ++dy)
			{
				if (dx == 0 && dy == 0)
				{
					continue;
				}
				Position neighborPos(pos.x + dx, pos.y + dy);
				if (isValidPosition(neighborPos))
				{
					positions.push_back(neighborPos);
				}
			}
		}
		return positions;
	}

	std::vector<Position> Map::getPositionsInRange(const Position& center, uint32_t range) const
	{
		std::vector<Position> positions;

		int32_t minX = std::max(0, static_cast<int32_t>(center.x) - static_cast<int32_t>(range));
		int32_t maxX
			= std::min(static_cast<int32_t>(_width) - 1, static_cast<int32_t>(center.x) + static_cast<int32_t>(range));
		int32_t minY = std::max(0, static_cast<int32_t>(center.y) - static_cast<int32_t>(range));
		int32_t maxY
			= std::min(static_cast<int32_t>(_height) - 1, static_cast<int32_t>(center.y) + static_cast<int32_t>(range));

		for (int32_t x = minX; x <= maxX; ++x)
		{
			for (int32_t y = minY; y <= maxY; ++y)
			{
				Position pos(static_cast<uint32_t>(x), static_cast<uint32_t>(y));
				if (pos != center)
				{
					positions.push_back(pos);
				}
			}
		}
		return positions;
	}
}
