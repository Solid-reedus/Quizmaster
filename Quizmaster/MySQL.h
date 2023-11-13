#ifndef MYSQL
#define MYSQL

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <sstream>

#include "Score.h"
#include "User.h"
#include <vector>

#ifndef QUESTION
#include "Question.h"
#endif

#ifndef CATEGORY
#include "Category.h"
#endif

#ifndef USER
#include "User.h"
#endif

enum GameMode
{
	gmNone = 0,
	gmTimed = 1,
	gmspeedrun = 2,
};

enum table
{
	tblQuestions,
	tblCategories,
	tblUserNames
};

class MySQL
{
	public:

	MySQL();
	~MySQL();

	std::vector<Question> GetQuestions(std::vector<Category>* m_categories, int m_amount);
	bool UserExsist(std::string m_name, std::string m_password);
	void MakeAcount(std::string m_name, std::string m_password);
	void AddNewScore(int m_userId, int m_score, int m_questionCount, int m_time, GameMode m_gameMode);
	std::vector<Score> GetScores(GameMode m_gameMode);
	User* GetUser(std::string m_name, std::string m_password);
	bool UserNameIsTaken(std::string m_name);
	bool HasEnoughQuestions(std::vector<Category>* m_categories, int m_count);
	std::vector<Category>* GetCategories();
	std::vector<std::string> GetAllRowsOf(table m_table);
	void DeleteRowOf(table m_table, std::string m_what);
	void MakeAdmin(std::string m_who);
	bool HasForeignKeys(table m_table, std::string m_who);
	bool CatagoryHasQuestionWithAnsers(std::string m_who);
	void MakeCategory(std::string m_name);
	void MakeQuestion(Question m_question, std::string m_category);

	private:

	bool ConnectToDb();
	std::string encryptDecrypt(const std::string& text, char key);

	const std::string dbHost = "localhost";
	const std::string dbUser = "root";
	const std::string dbPassword = "";
	const std::string dbName = "quizmaster";

	sql::mysql::MySQL_Driver* driver;
	sql::Connection* con;

};



#endif
