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



class MySQL
{
	public:

	MySQL();
	~MySQL();

	void Free();

	std::vector<Question> GetQuestions(std::string m_category);
	private:

	bool ConnectToDb();

	void MakeAcount(std::string m_name, std::string m_password);
	User* GetUser(std::string m_name, std::string m_password);
	std::string encryptCaesarCipher(const std::string& plaintext, int shift);


	const std::string dbHost = "localhost";
	const std::string dbUser = "root";
	const std::string dbPassword = "";
	const std::string dbName = "quizmaster";

	sql::mysql::MySQL_Driver* driver;
	sql::Connection* con;

};



#endif
