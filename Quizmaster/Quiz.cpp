#include "quiz.h"


Quiz::Quiz()
{
	correctAnswers = 0;
	mysql = new MySQL();
	
}

Quiz::~Quiz()
{
	delete mysql;
}

void Quiz::StartQuiz(std::string m_category)
{
	//questions = *(mysql->GetQuestions(""));
	mysql->GetQuestions(m_category);
}

Question* Quiz::GetQuestion(int m_index)
{
	if (m_index < questions.size())
	{
		return &questions[m_index];
	}

	printf("index %d from GetQuestion is out of index", m_index);
	return nullptr;
}