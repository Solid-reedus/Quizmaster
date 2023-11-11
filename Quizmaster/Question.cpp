#include "Question.h"


Question::Question(std::string m_title, std::vector<Answer> m_answers)
{
	title = m_title;
	answers = m_answers;
	value = 1;
}

Question::~Question()
{
	title = "";
	answers.clear();
}

Answer::Answer(std::string m_text, bool m_isTrue)
{
	text = m_text;
	isTrue = m_isTrue;
}

Answer::~Answer()
{
	text = "";
	isTrue = NULL;
}

void Question::AddAnswer(std::string m_text, bool m_isTrue)
{
	answers.push_back(Answer(m_text, m_isTrue));
}