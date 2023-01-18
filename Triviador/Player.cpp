#include "Player.h"

Player::Player(const std::string& username, const std::string& password, const uint16_t& indexVector)
	:m_username(username),
	m_password(password),
	m_indexVector(indexVector)
{
}

std::shared_ptr<Region> Player::GetBase() const
{
	const uint16_t baseIndex = 0;
	return m_territory[baseIndex];
}

std::vector<std::shared_ptr<Region>> Player::GetTerritory() const
{
	return m_territory;
}

uint16_t Player::GetIndexVector() const
{
	return m_indexVector;
}

uint16_t Player::TotalScore() const
{
	uint16_t totalScore = 0;
	for (const auto& region : m_territory)
		totalScore = totalScore + region->GetScore();
	return totalScore;
}

void Player::AddRegion(std::shared_ptr<Region> region)
{
	m_territory.push_back(region);
}

void Player::RemoveRegion(std::shared_ptr<Region> region)
{
	m_territory.erase(std::find(m_territory.begin(), m_territory.end(), region));
}

bool Player::VerifyVecinity(const std::pair<uint16_t, uint16_t>& indexes)
{
	std::pair<uint16_t, uint16_t> difference;
	for (const auto& region : m_territory)
		if (region->GetCoordinates() == indexes)
			return false;
	for (const auto& region : m_territory)
	{
		if (std::abs(indexes.first - region->GetCoordinates().first) == 0 && std::abs(indexes.second - region->GetCoordinates().second) == 1)
			return true;
		if (std::abs(indexes.first - region->GetCoordinates().first) == 1 && std::abs(indexes.second - region->GetCoordinates().second) == 0)
			return true;
	}
	return false;
}

bool Player::VerifyRegion(const std::pair<uint16_t, uint16_t>& indexes) const
{
	for (const auto& region : m_territory)
		if (region->GetCoordinates() == indexes && !region->GetIsBase())
			return true;
	return false;
}

bool Player::EligibleForAvantages() const
{
	const uint16_t minScoreAvantage = 200;
	for (const auto& region : m_territory)
		if (region->GetScore() >= minScoreAvantage && !region->GetIsBase())
			return true;
	return false;
}

void Player::Conquering(std::shared_ptr<Player> opponent)
{
	for (auto region : opponent->GetTerritory())
	{
		region->SetOwned(this->m_indexVector + 1);
		this->m_territory.push_back(region);
	}
	opponent->Elimination();
}

void Player::Elimination()
{
	m_territory.clear();
}

std::ostream& operator<<(std::ostream& out, std::shared_ptr<Player> player)
{
	out << player->m_username << "[" << player->m_indexVector + 1 << "]";
	return out;
}