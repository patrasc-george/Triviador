#include "Region.h"

Region::Region(const std::pair<uint16_t, uint16_t>& coordinates)
	:m_isBase(false),
	m_score(100),
	m_owned(false),
	m_coordinates(coordinates)
{
}

bool Region::GetIsBase() const
{
	return m_isBase;
}

uint16_t Region::GetOwned() const
{
	return m_owned;
}

uint16_t Region::GetScore() const
{
	return m_score;
}

std::pair<uint16_t, uint16_t> Region::GetCoordinates() const
{
	return m_coordinates;
}

void Region::SetOwned(const uint16_t& index)
{
	m_owned = index;
}

void Region::SetBase(const uint16_t& index)
{
	const uint16_t increment = 300;
	if (!m_isBase)
	{
		m_owned = index;
		m_isBase = true;
		m_score = increment;
	}
}

void Region::SetIsNotBase()
{
	m_isBase = false;
}

void Region::IncrementScore()
{
	const uint16_t increment = 100;
	m_score += 100;
}

void Region::DecrementScore()
{
	const uint16_t decrement = 100;
	m_score -= decrement;
}

bool Region::operator==(Region region)
{
	if (m_coordinates == region.GetCoordinates())
		return true;
	return false;
}
