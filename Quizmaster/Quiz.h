#ifndef QUIZ
#define QUIZ

#include "Question.h"

//#ifdef MYSQL
//#endif
#include "MySQL.h"


#ifndef BASE_HEADER
#include "BaseHeader.h"
#endif



class Quiz
{
	public:
	Quiz();
	~Quiz();
	void Free();

	void StartQuiz(std::string m_category);
	Question* GetQuestion(int m_index);


	private:
	int correctAnswers;
	std::vector<Question> questions;
	MySQL* mysql;


};


#endif
