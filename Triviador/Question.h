#pragma once

#include <iostream>
#include <vector>

class Question
{
public:
	Question(const std::string& question, const std::vector<std::string>& answers, const uint16_t& correctAnswer);

	std::string GetQuestion() const;
	std::vector<std::string> GetAnswers() const;
	std::string GetCorrectAnswer() const;
	uint16_t GetIndexCorrectAnswer() const;
	bool GetPrint();

	void SetCorrectAnswer(const uint16_t& correctAnswer);
	void SetAnswers(const std::vector<std::string>& answers);
	void RemoveAnswer(const uint16_t& index);

	friend std::ostream& operator<<(std::ostream& out, const Question& question);

private:
	std::string m_question;
	std::vector<std::string> m_answers;
	uint16_t m_correctAnswer;
	bool m_print;
};

