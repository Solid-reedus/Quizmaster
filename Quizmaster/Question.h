#ifndef QUESTION
#define QUESTION

#ifndef BASE_HEADER
#include "BaseHeader.h"
#endif

class Answer
{
public:
	Answer(std::string m_text, bool m_isTrue);
	std::string text;
	bool isTrue;
	~Answer();
};



class Question
{
	public:

	Question(std::string m_title, std::vector<Answer> m_answers);
	void AddAnswer(std::string m_text, bool m_isTrue);
	std::string title;
	std::vector<Answer> answers;
	~Question();
};



#endif
