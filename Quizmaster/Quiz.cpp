#include "quiz.h"

const Question NullQuestion("", {});

Quiz::Quiz()
{
	//mysql = MySQL();
}

Quiz::~Quiz()
{

}


Question Quiz::GetQuestion(int m_index)
{
	if (m_index < questions.size())
	{
		return questions[m_index];
	}

	printf("index %d from GetQuestion is out of index", m_index);
	return Question(NullQuestion);
}