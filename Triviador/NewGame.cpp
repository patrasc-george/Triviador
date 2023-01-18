#include <fstream>
#include <chrono>
#include <sstream>
#include <algorithm>
#include <tuple>
#include <random>

#include "NewGame.h"

void NewGame::ReadQuestion()
{
	std::ifstream f("Questions.txt");
	std::string question, answer;
	std::vector<std::string> answers;
	uint16_t m_correctAnswer;
	const uint16_t questionMultipleChoice = 300;
	const uint16_t numericalQuestion = 400;
	uint16_t lineCounter = 0;
	while (lineCounter < questionMultipleChoice)
	{
		const uint16_t numberAnswers = 4;
		const uint16_t nextQuestion = 6;
		std::getline(f, question);
		for (int i = 0; i < numberAnswers; i++)
		{
			std::getline(f, answer);
			answers.push_back(answer);
		}
		std::getline(f, answer);
		Question questionObject(question, answers, std::stoi(answer));
		m_questions.push_back(questionObject);
		lineCounter = lineCounter + nextQuestion;
		answers.clear();
	}
	while (lineCounter < numericalQuestion)
	{
		const uint16_t indexCorrectAnswer = 0;
		const uint16_t nextQuestion = 2;
		std::getline(f, question);
		std::getline(f, answer);
		answers.push_back(answer);
		Question questionObject(question, answers, indexCorrectAnswer);
		m_questions.push_back(questionObject);
		lineCounter = lineCounter + nextQuestion;
		answers.clear();
	}
	f.close();
}

void VerifyNumberPlayers(uint16_t& numberPlayers)
{
	const uint16_t minimPlayers = 2;
	const uint16_t maximPlayers = 4;
	std::cout << std::endl;
	while (numberPlayers < minimPlayers || numberPlayers > maximPlayers)
	{
		std::cout << "At least 2 players and a maximum of 4 can participate in the game." << std::endl;
		std::cout << "How many players participate in the game?" << std::endl;
		std::cin >> numberPlayers;
		std::cout << std::endl;
	}
}

void NewGame::InitializeMapAndRounds(const uint16_t& numberPlayers)
{
	switch (numberPlayers)
	{
	case 2:
		m_map = Map(3, 3);
		roundsNumber = 5;
		break;
	case 3:
		m_map = Map(5, 3);
		roundsNumber = 4;
		break;
	case 4:
		m_map = Map(6, 4);
		roundsNumber = 5;
		break;
	}
}
void Login(std::vector<std::shared_ptr<Player>>& m_players, const uint16_t& numberPlayers)
{
	std::string username, password;
	for (int i = 0; i < numberPlayers; i++)
	{
		std::cout << "LOGIN" << std::endl;
		std::cout << "Username: ";
		std::cin >> username;
		std::cout << "Password: ";
		std::cin >> password;
		std::shared_ptr<Player> player = std::make_shared<Player>(username, password, i);
		m_players.push_back(player);
		std::cout << std::endl;
	}
}

void NewGame::InitializeGame()
{
	uint16_t numberPlayers;
	std::cout << "How many players participate in the game?" << std::endl;
	std::cin >> numberPlayers;
	VerifyNumberPlayers(numberPlayers);
	InitializeMapAndRounds(numberPlayers);
	Login(m_players, numberPlayers);
}

Question NewGame::GetNumericalQuestion()
{
	const uint16_t numberNumericalQuestion = 50;
	const uint16_t numberQuestionMultipleChoice = 50;
	srand(time(NULL));
	uint16_t index;
	do
	{
		index = rand() % numberNumericalQuestion + numberQuestionMultipleChoice;
	} while (m_questions[index].GetPrint());
	return m_questions[index];
}

Question NewGame::GetQuestionMultipleChoice()
{
	const uint16_t numberQuestionMultipleChoice = 50;
	srand(time(NULL));
	uint16_t index;
	do
	{
		index = rand() % numberQuestionMultipleChoice;
	} while (m_questions[index].GetPrint());
	return m_questions[index];
}

void ReadCoordinates(uint16_t& coordinate1, uint16_t& coordinate2)
{
	std::cout << "Introduce first coordinate: ";
	std::cin >> coordinate1;
	std::cout << "Introduce second coordinate: ";
	std::cin >> coordinate2;
}

void NewGame::VerifyAvantageCoordinates(std::shared_ptr<Player> player, uint16_t& coordinate1, uint16_t& coordinate2)
{
	const uint16_t minScoreAvantage = 200;
	std::cout << m_map;
	std::cout << player << ", choose a region, based on coordinates, that you want to decrement to be able to use an advantage." << std::endl;
	ReadCoordinates(coordinate1, coordinate2);
	while (coordinate1 > m_map.size().first - 1 || coordinate2 > m_map.size().second - 1)
	{
		std::cout << "The region does not exist. Please choose a valid region based on coordinates." << std::endl;
		ReadCoordinates(coordinate1, coordinate2);
	}
	while (!player->VerifyRegion(std::make_pair(coordinate1, coordinate2)))
	{
		std::cout << "The selected region is not yours. Please choose a region which is yours, based on coordinates." << std::endl;
		ReadCoordinates(coordinate1, coordinate2);
	}
	while (m_map.GetMap()[coordinate1][coordinate2]->GetScore() < minScoreAvantage)
	{
		std::cout << "The selected region doesn't have a high enough score. Please choose a region which has at least a score of 200, based on coordinates." << std::endl;
		ReadCoordinates(coordinate1, coordinate2);
	}
}

void NewGame::UseAvantage(std::shared_ptr<Player> player, const Question& question)
{
	if (player->EligibleForAvantages())
	{
		uint16_t coordinate1, coordinate2;
		VerifyAvantageCoordinates(player, coordinate1, coordinate2);
		m_map.GetMap()[coordinate1][coordinate2]->DecrementScore();
		Avantage avantage;
		avantage.Menu(question);
	}
	else
		std::cout << "You don't have at least one region that has enough score to be able to use the advantages." << std::endl;
}

template <typename T>
T NewGame::AskForInput(std::shared_ptr<Player> player, const Question& question)
{
	const uint16_t numericalQuestion = 1;
	std::string answer;
	T newAnswer;
	std::cout << player << ", please input your answer: ";
	std::cin >> answer;
	if (answer == "+")
	{
		UseAvantage(player, question);
		std::cout << player << ", please input your answer: ";
		std::cin >> newAnswer;
	}
	else
		if (question.GetAnswers().size() == numericalQuestion)
			newAnswer = std::stoi(answer);
		else
			newAnswer = answer[0];
	return newAnswer;
}

std::tuple<uint16_t, int, int> NewGame::IndexAnswerTime(const Question& question, std::shared_ptr<Player> player, const uint16_t& index)
{
	int answer;
	int time;

	using Clock = std::chrono::high_resolution_clock;
	auto start = Clock::now();
	answer = AskForInput<int>(player, question);
	auto end = Clock::now();

	answer = answer - std::stoi(question.GetCorrectAnswer());
	answer = std::abs(answer);

	time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	return std::make_tuple(index, answer, time);
}

bool compareTuples(const std::tuple<uint16_t, int, int>& a, const std::tuple<uint16_t, int, int>& b)
{
	if (std::get<1>(a) == std::get<1>(b))
	{
		return std::get<2>(a) <= std::get<2>(b);
	}
	return std::get<1>(a) < std::get<1>(b);
}

void NewGame::TopPlayersForOneQuestion(const Question& question, std::vector<std::shared_ptr<Player>>& players)
{
	std::vector<std::tuple<uint16_t, int, int>> forSorting;
	std::cout << question;
	for (int index = 0; index < players.size(); index++)
		forSorting.push_back(IndexAnswerTime(question, players[index], index));
	std::sort(forSorting.begin(), forSorting.end(), compareTuples);

	std::vector<std::shared_ptr<Player>> sortedPlayers;

	for (int i = 0; i < forSorting.size(); i++)
		sortedPlayers.push_back(players[std::get<0>(forSorting[i])]);
	players = sortedPlayers;
}

void NewGame::StageChooseBase()
{
	std::cout << std::endl << std::endl << "STAGE 1: CHOOSE BASES" << std::endl << std::endl;
	auto copyPlayers = std::make_unique<std::vector<std::shared_ptr<Player>>>(m_players);
	Question question = GetNumericalQuestion();
	TopPlayersForOneQuestion(question, *copyPlayers);
	std::cout << std::endl << "Correct Answer: " << question.GetCorrectAnswer() << std::endl;
	for (auto player : *copyPlayers)
	{
		uint16_t coordinate1 = 0, coordinate2 = 0;
		std::cout << m_map;
		std::cout << player << ", choose region based on coordinates." << std::endl;
		ReadCoordinates(coordinate1, coordinate2);

		while (coordinate1 > m_map.size().first - 1 || coordinate2 > m_map.size().second - 1)
		{
			std::cout << "The region does not exist. Please choose a valid region based on coordinates." << std::endl;
			ReadCoordinates(coordinate1, coordinate2);
		}
		while (m_map.GetMap()[coordinate1][coordinate2]->GetOwned())
		{
			std::cout << "Base already occupied. Please choose an unocuppied region based on coordinates." << std::endl;
			ReadCoordinates(coordinate1, coordinate2);
		}
		std::shared_ptr<Region> region = m_map.GetMap()[coordinate1][coordinate2];
		region->SetBase(player->GetIndexVector() + 1);
		player->AddRegion(region);
		std::cout << "Base initialised with succes!" << std::endl << std::endl;
	}
	std::cout << "All the bases have been chosen!" << std::endl << std::endl << "Final map:";
	std::cout << m_map << std::endl << std::endl;
}

void NewGame::StageChoseRegion()
{
	std::cout << std::endl << std::endl << "STAGE 2: CHOOSE REGIONS" << std::endl << std::endl;
	do
	{
		auto copyPlayers = std::make_unique<std::vector<std::shared_ptr<Player>>>(m_players);
		Question question = GetNumericalQuestion();
		TopPlayersForOneQuestion(question, *copyPlayers);
		std::cout << std::endl << "Correct Answer: " << question.GetCorrectAnswer() << std::endl << std::endl;

		uint16_t placeRanking = 0;
		for (auto player : *copyPlayers)
		{
			uint16_t numberOfRegionsToBeSelected = m_players.size() - placeRanking;

			while (numberOfRegionsToBeSelected && m_map.CheckIfEmptyRegions())
			{
				uint16_t coordinate1 = 0, coordinate2 = 0;

				std::cout << m_map;
				std::cout << player << ", choose region based on coordinates." << std::endl;
				ReadCoordinates(coordinate1, coordinate2);
				while (coordinate1 > m_map.size().first - 1 || coordinate2 > m_map.size().second - 1)
				{
					std::cout << "The region does not exist. Please choose a valid region based on coordinates." << std::endl;
					ReadCoordinates(coordinate1, coordinate2);
				}
				while (m_map.GetMap()[coordinate1][coordinate2]->GetOwned())
				{
					std::cout << "Region already occupied. Please choose an unocuppied region based on coordinates." << std::endl;
					ReadCoordinates(coordinate1, coordinate2);

				}
				std::shared_ptr<Region> region = m_map.GetMap()[coordinate1][coordinate2];
				region->SetOwned(player->GetIndexVector() + 1);
				player->AddRegion(region);
				std::cout << "Region added with succes!" << std::endl << std::endl;
				numberOfRegionsToBeSelected--;
			}
			placeRanking++;
		}
	} while (m_map.CheckIfEmptyRegions());
	std::cout << "All regions are owned!" << std::endl << "Final map:" << std::endl;
	std::cout << m_map << std::endl << std::endl;
}

bool NewGame::VerifyGameContinues() const
{
	uint16_t counter = 0;
	const uint16_t minPlayers = 1;
	for (const auto& player : m_players)
	{
		if (player->GetTerritory().size())
			counter++;
		if (counter > minPlayers)
			return true;
	}
	return false;
}

void NewGame::VerifyAttackCoordinates(std::shared_ptr<Player> player, uint16_t& coordinate1, uint16_t& coordinate2)
{
	std::cout << m_map;
	std::cout << player << ", choose a region in your vecinity to attack, based on coordinates." << std::endl;
	ReadCoordinates(coordinate1, coordinate2);
	while (coordinate1 > m_map.size().first - 1 || coordinate2 > m_map.size().second - 1)
	{
		std::cout << "The region does not exist. Please choose a valid region based on coordinates." << std::endl;
		ReadCoordinates(coordinate1, coordinate2);
	}
	while (!player->VerifyVecinity(std::make_pair(coordinate1, coordinate2)))
	{
		std::cout << "The selected region is not in your vicinity. Please choose a region in your vecinity, based on coordinates." << std::endl;
		ReadCoordinates(coordinate1, coordinate2);
	}
	std::cout << std::endl;
}

bool VerifyAnswer(const char& answer, const Question& question)
{
	const uint16_t aASCII = 97;
	if (int(answer) - aASCII == question.GetIndexCorrectAnswer())
		return true;
	return false;
}

void NewGame::DetermineWinner(std::shared_ptr<Player> player, std::shared_ptr<Region> region)
{
	std::shared_ptr<Player> opponent = m_players[region->GetOwned() - 1];
	Question question = GetQuestionMultipleChoice();
	std::pair<char, char> answers;
	std::cout << question;
	answers.first = AskForInput<char>(player, question);
	answers.second = AskForInput<char>(opponent, question);
	std::cout << std::endl << "Correct Answer: " << question.GetCorrectAnswer() << std::endl;
	if (VerifyAnswer(answers.first, question))
	{
		std::vector<std::shared_ptr<Player>> players = { player, opponent };
		if (VerifyAnswer(answers.second, question))
		{
			std::cout << std::endl;
			Question numericalQuestion = GetNumericalQuestion();
			TopPlayersForOneQuestion(numericalQuestion, players);
			std::cout << std::endl << "Correct Answer: " << numericalQuestion.GetCorrectAnswer() << std::endl;
		}
		if (player == players[0])
		{
			std::cout << player << " won the duel and " << opponent << "'s region has been decremented." << std::endl;
			region->DecrementScore();
			if (!region->GetScore())
			{
				std::cout << player << " conquered the region." << std::endl << std::endl;;
				region->IncrementScore();
				region->SetOwned(player->GetIndexVector() + 1);
				player->AddRegion(region);
				opponent->RemoveRegion(region);
			}
			else
				player->GetBase()->IncrementScore();
			if (region->GetIsBase())
				if (region->GetOwned() - 1 == player->GetIndexVector())
				{
					std::cout << player << " conquered " << opponent << "'s base and all his territories.";
					region->SetIsNotBase();
					player->Conquering(opponent);
				}
				else
					DetermineWinner(player, region);
			return;
		}
	}
	if (VerifyAnswer(answers.second, question))
	{
		std::cout << opponent << " successfully defended his region and it was increased." << std::endl << std::endl;;
		region->IncrementScore();
		return;
	}
	std::cout << "No player answered the question correctly." << std::endl << std::endl;;
}

void NewGame::StageDuels()
{
	std::cout << std::endl << std::endl << std::endl << "STAGE 3: DUELS" << std::endl << std::endl;
	std::vector<std::shared_ptr<Player>> shufflePlayers = m_players;
	uint16_t currentRound = 1;
	while (currentRound <= roundsNumber && VerifyGameContinues())
	{
		std::cout << std::endl << "ROUND " << currentRound;
		std::shuffle(std::begin(shufflePlayers), std::end(shufflePlayers), std::random_device{});
		for (auto player : shufflePlayers)
			if (player->TotalScore())
			{
				uint16_t coordinate1 = 0, coordinate2 = 0;
				VerifyAttackCoordinates(player, coordinate1, coordinate2);
				std::shared_ptr<Region> region = m_map.GetMap()[coordinate1][coordinate2];
				DetermineWinner(player, region);
			}
		currentRound++;
	}
}

void SortScores(std::vector<std::shared_ptr<Player>>& m_players)
{
	for (int i = 1; i < m_players.size(); i++)
	{
		uint16_t iInitial = i;
		for (int j = i - 1; j >= 0; j--)
			if (m_players[i]->TotalScore() > m_players[j]->TotalScore())
			{
				std::swap(m_players[i], m_players[j]);
				i--;
			}
			else
			{
				i = iInitial;
				break;
			}
		i = iInitial;
	}
}

void NewGame::Ranking()
{
	SortScores(m_players);
	std::cout << std::endl << "GAME OVER" << std::endl;
	std::cout << m_players[0] << " won the game!" << std::endl << std::endl;
	std::cout << "RANKING" << std::endl;
	for (int i = 0; i < m_players.size(); i++)
		std::cout << i + 1 << ". " << m_players[i] << " - " << m_players[i]->TotalScore() << std::endl;
}