#ifndef MYSQL
#define MYSQL

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <sstream>

#include "User.h"
#include <vector>

#ifndef QUESTION
#include "Question.h"
#endif

#ifndef USER
#include "User.h"
#endif


class MySQL
{
	public:

	MySQL();
	~MySQL();

	std::vector<Question> GetQuestions(std::string m_category);
	bool UserExsist(std::string m_name, std::string m_password);
	void MakeAcount(std::string m_name, std::string m_password);
	User* GetUser(std::string m_name, std::string m_password);
	bool UserNameIsTaken(std::string m_name);
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
