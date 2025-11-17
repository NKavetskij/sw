#pragma once

#include <cmath>
#include <cstdint>
#include <functional>

namespace sw::core
{
	struct Position
	{
		uint32_t x;
		uint32_t y;

		Position(uint32_t x = 0, uint32_t y = 0) :
				x(x),
				y(y)
		{}

		bool operator==(const Position& other) const
		{
			return x == other.x && y == other.y;
		}

		bool operator!=(const Position& other) const
		{
			return !(*this == other);
		}

		double distanceTo(const Position& other) const
		{
			int32_t dx = static_cast<int32_t>(x) - static_cast<int32_t>(other.x);
			int32_t dy = static_cast<int32_t>(y) - static_cast<int32_t>(other.y);
			return std::sqrt(dx * dx + dy * dy);
		}

		bool isNeighborTo(const Position& other) const
		{
			auto dx = std::abs(static_cast<int32_t>(x) - static_cast<int32_t>(other.x));
			auto dy = std::abs(static_cast<int32_t>(y) - static_cast<int32_t>(other.y));
			return dx <= 1 && dy <= 1 && !(dx == 0 && dy == 0);
		}
	};
}

namespace std
{
	template <>
	struct hash<sw::core::Position>
	{
		size_t operator()(const sw::core::Position& pos) const
		{
			return hash<uint32_t>()(pos.x) ^ (hash<uint32_t>()(pos.y) << 1);
		}
	};
}
