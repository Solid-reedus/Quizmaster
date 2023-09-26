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

	void StartQuiz(std::vector<Category>* m_categories, int m_amount);
	Question* GetQuestion(int m_index);
	void SetMySQL(MySQL* m_mysql);
	int GetQuestionCount();
	bool HasEnoughQuestions(std::vector<Category>* m_categories, int m_count);

	private:
	int correctAnswers;
	std::vector<Question> questions;
	MySQL* mysql;


};


#endif
