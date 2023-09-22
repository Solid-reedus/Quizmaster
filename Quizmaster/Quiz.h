#ifndef QUIZ
#define QUIZ

#include "Question.h"

#ifndef MYSQL
#include "MySQL.h"
#endif


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
	void SetMySQL(MySQL* m_mysql);


	private:
	int correctAnswers;
	std::vector<Question> questions;
	MySQL* mysql;


};


#endif
