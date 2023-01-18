#include "Map.h"

Map::Map(const uint8_t& rows, const uint8_t& columns)
{
	m_map.resize(rows, std::vector<std::shared_ptr<Region>>(columns));
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			m_map[i][j] = std::make_shared<Region>(std::make_pair(i, j));
}

std::vector<std::vector<std::shared_ptr<Region>>> Map::GetMap() const
{
	return m_map;
}

std::pair<uint16_t, uint16_t> Map::size() const
{
	return std::make_pair(m_map.size(), m_map[0].size());
}

bool Map::CheckIfEmptyRegions() const
{
	for (int i = 0; i < m_map.size(); i++)
	{
		for (int j = 0; j < m_map[i].size(); j++)
		{
			if (!m_map[i][j]->GetOwned())
			{
				return true;
			}
		}
	}
	return false;
}

std::ostream& operator<<(std::ostream& out, Map map)
{
	out << std::endl << std::endl;
	out << "MAP:" << std::endl;
	out << "Legend: R = region, B = Base, S = Score, O = Owned" << std::endl << std::endl;
	for (auto row = 0; row < map.m_map.size(); row++)
	{
		for (auto column = 0; column < map.m_map[row].size(); column++)
		{
			out << "R:" << row << column;
			out << " B:" << map.m_map[row][column]->GetIsBase();
			out << " S:" << map.m_map[row][column]->GetScore();
			out << " O:" << map.m_map[row][column]->GetOwned();
			out << "  ";
		}
		out << std::endl;
	}
	out << std::endl;
	return out;
}