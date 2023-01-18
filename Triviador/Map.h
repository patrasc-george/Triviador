#pragma once

#include <vector>

#include "Region.h"

class Map
{
public:
	Map() = default;
	Map(const uint8_t& rows, const uint8_t& columns);

	std::vector<std::vector<std::shared_ptr<Region>>> GetMap() const;
	std::pair<uint16_t, uint16_t> size() const;

	bool CheckIfEmptyRegions() const;

	friend std::ostream& operator<<(std::ostream& out, Map map);

private:
	std::vector<std::vector<std::shared_ptr<Region>>> m_map;
};

