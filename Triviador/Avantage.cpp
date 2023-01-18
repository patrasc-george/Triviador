#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <cmath>

#include "Avantage.h"

void Avantage::Menu(const Question& question) const
{
	uint16_t x;
	const uint16_t numericalQuestion = 1;
	std::cout << std::endl << "AVANTAJE" << std::endl;
	if (question.GetAnswers().size() == numericalQuestion)
	{
		std::cout << "1. Receives 4 answers, one of which is correct." << std::endl;
		std::cout << "2. Receives a value close to the correct answer." << std::endl;
		std::cout << "3. Exit" << std::endl;
		std::cout << "Choose an option: ";
		std::cin >> x;
		switch (x)
		{
		case 1:
			MultipleChoice(question);
			break;
		case 2:
			CloseValue(question);
			break;
		}
	}
	else
	{
		std::cout << "1. Eliminate 2 wrong answers." << std::endl;
		std::cout << "2. Exit" << std::endl;
		std::cin >> x;
		switch (x)
		{
		case 1:
			RemoveWrongAnswers(question);
			break;
		}
	}
}

void Avantage::MultipleChoice(Question question) const
{
	const uint16_t parity = 2;
	const uint16_t nrAnswers = 4;
	int answer = std::stoi(question.GetAnswers()[0]);
	uint16_t nrDigits = std::floor(std::log10(answer) + 1);
	if (nrDigits % parity == 1)
		nrDigits++;
	nrDigits = nrDigits / 2;
	int difference = answer / (2 * nrDigits);
	int min = answer - difference;
	int max = answer + difference;
	while (max - min < 3)
		max++;
	std::string randomAnswer = std::to_string(answer);
	std::vector<std::string> answers;
	answers.push_back(randomAnswer);
	std::mt19937 generator(std::random_device{}());
	std::uniform_int_distribution<int> distribution(min, max);
	while (answers.size() < nrAnswers)
	{
		do
		{
			answer = distribution(generator);
			randomAnswer = std::to_string(answer);
		} while (std::find(answers.begin(), answers.end(), randomAnswer) != answers.end());
		answers.push_back(randomAnswer);
	}
	randomAnswer = question.GetAnswers()[0];
	std::shuffle(answers.begin(), answers.end(), std::mt19937{ std::random_device{}() });
	answers.push_back("");
	question.SetAnswers(answers);
	question.SetCorrectAnswer(std::find(answers.begin(), answers.end(), randomAnswer) - answers.begin());
	std::cout << std::endl << question;
}

void Avantage::CloseValue(const Question& question) const
{
	const uint16_t parity = 2;
	int answer = std::stoi(question.GetAnswers()[0]);
	uint16_t nrDigits = std::floor(std::log10(answer) + 1);
	int difference = answer / (2 * nrDigits);
	int min = answer - difference;
	int max = answer + difference;
	while (max - min < parity)
		max++;
	std::mt19937 generator(std::random_device{}());
	std::uniform_int_distribution<int> distribution(min, max);
	std::cout << std::endl << question.GetQuestion();
	std::cout << "A value close to the correct answer: " << distribution(generator) << std::endl << std::endl;
}

void Avantage::RemoveWrongAnswers(Question question) const
{
	const uint16_t aASCII = 97;
	uint16_t answersRemoved = 0;
	std::vector<uint16_t> indexQuestions = { 0, 1, 2, 3 };
	std::shuffle(indexQuestions.begin(), indexQuestions.end(), std::mt19937{ std::random_device{}() });
	for (const auto& i : indexQuestions)
	{
		if (i != question.GetIndexCorrectAnswer())
		{
			question.RemoveAnswer(i);
			answersRemoved++;
		}
		if (answersRemoved == 2)
			break;
	}
	std::cout << std::endl << question.GetQuestion() << std::endl;
	for (int i = 0; i < question.GetAnswers().size(); i++)
		if (question.GetAnswers()[i] != "")
			std::cout << (char)(i + aASCII) << ". " << question.GetAnswers()[i] << "   ";
	std::cout << std::endl;
}