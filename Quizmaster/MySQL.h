#ifndef MYSQL
#define MYSQL

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

class MySQL
{
	public:

	MySQL();
	~MySQL();

	void Free();

	private:

	bool ConnectToDb();


	const std::string db_host = "localhost";
	const std::string db_user = "root";
	const std::string db_password = "";
	const std::string db_name = "quizmaster";

	sql::mysql::MySQL_Driver* driver;
	sql::Connection* con;


};



#endif
