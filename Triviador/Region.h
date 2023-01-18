#pragma once

#include <iostream>
#include <cstdint>

class Region
{
public:
	Region() = default;
	Region(const std::pair<uint16_t, uint16_t>& coordinates);

	bool GetIsBase() const;
	uint16_t GetOwned() const;
	uint16_t GetScore() const;
	std::pair<uint16_t, uint16_t> GetCoordinates() const;

	void SetOwned(const uint16_t& index);
	void SetBase(const uint16_t& index);
	void SetIsNotBase();
	void IncrementScore();
	void DecrementScore();

	bool operator==(Region region);

private:
	bool m_isBase;
	uint16_t m_owned;
	uint16_t m_score;
	std::pair<uint16_t, uint16_t> m_coordinates;
};

