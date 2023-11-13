#include "quiz.h"

uint8_t const SUPER_QUESTION_INTERVAL = 20;


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

int Quiz::getRandomNumber(int min, int max)
{
	static std::mt19937 generator(std::random_device{}());
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(generator);
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

		//modify values from questions
		//if question is lesser than the interval then return early
		if ((questions.size() / SUPER_QUESTION_INTERVAL) < 1)
		{
			return;
		}

		for (size_t i = SUPER_QUESTION_INTERVAL; i < questions.size(); i+= SUPER_QUESTION_INTERVAL)
		{
			Uint8 index = getRandomNumber(i, i + SUPER_QUESTION_INTERVAL);
			if (index > questions.size() - 1)
			{
				index = questions.size() - 1;
			}
			questions[index].value = 2;
		}

	}
	else
	{
		printf("unable to start quiz no ptr to mysql");
	}
}

bool Quiz::HasEnoughQuestions(std::vector<Category>* m_categories, int m_count)
{
	return mysql->HasEnoughQuestions(m_categories, m_count);
}

int Quiz::GetQuestionCount()
{
	return questions.size();
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