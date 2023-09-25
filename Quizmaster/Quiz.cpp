#include "quiz.h"


Quiz::Quiz()
{
	correctAnswers = 0;
	//mysql = new MySQL();
	mysql = nullptr;
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
	//delete mysql;

}

void Quiz::SetMySQL(MySQL* m_mysql)
{
	mysql = m_mysql;
}


void Quiz::StartQuiz(std::vector<Category>* m_categories, int m_amount)
{
	if (mysql != nullptr)
	{
		questions = mysql->GetQuestions(m_categories, m_amount);
	}
	else
	{
		printf("unable to start quiz no ptr to mysql");
	}
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