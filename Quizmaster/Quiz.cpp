#include "quiz.h"


Quiz::Quiz()
{
	correctAnswers = 0;
	mysql = new MySQL();
	
}

Quiz::~Quiz()
{
	Free();

}

void Quiz::Free()
{
	correctAnswers = 0;
	questions.clear();
	mysql = nullptr;
	delete mysql;

}

void Quiz::StartQuiz(std::string m_category)
{
	questions = mysql->GetQuestions(m_category);
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