#pragma once

#include "Question.h"
#include "Player.h"
#include "Map.h"
#include "Avantage.h"

class NewGame
{
public:
	void ReadQuestion();
	void InitializeMapAndRounds(const uint16_t& numberPlayers);
	void InitializeGame();

	Question GetNumericalQuestion();
	Question GetQuestionMultipleChoice();

	void VerifyAvantageCoordinates(std::shared_ptr<Player> player, uint16_t& coordinate1, uint16_t& coordinate2);
	void UseAvantage(std::shared_ptr<Player> player, const Question& question);
	
	template <typename T>
	T AskForInput(std::shared_ptr<Player> player, const Question& question);
	std::tuple<uint16_t, int, int> IndexAnswerTime(const Question& question, std::shared_ptr<Player> player, const uint16_t& index);
	void TopPlayersForOneQuestion(const Question& question, std::vector<std::shared_ptr<Player>>& players);
	
	void StageChooseBase();
	void StageChoseRegion();

	bool VerifyGameContinues() const;
	void VerifyAttackCoordinates(std::shared_ptr<Player> player, uint16_t& coordinate1, uint16_t& coordinate2);
	void DetermineWinner(std::shared_ptr<Player> player, std::shared_ptr<Region> region);
	void StageDuels();

	void Ranking();

private:
	Map m_map;
	std::vector<Question> m_questions;
	std::vector<std::shared_ptr<Player>> m_players;
	uint16_t roundsNumber;
};
