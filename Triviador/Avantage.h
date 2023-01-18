#pragma once
#include "Question.h"

class Avantage
{
public:
	void Menu(const Question& question) const;

	void MultipleChoice(Question question) const;
	void CloseValue(const Question& question) const;
	void RemoveWrongAnswers(Question question) const;
};

