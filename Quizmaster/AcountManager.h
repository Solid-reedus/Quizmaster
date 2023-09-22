#ifndef ACOUNT_MANAGER
#define ACOUNT_MANAGER

#ifndef MYSQL
#include "MySQL.h"
#endif

#ifndef USER
#include "User.h"
#endif

class AcountManager
{
	public:
	AcountManager();
	~AcountManager();

	void SetMySQL(MySQL* m_mysql);
	bool UserExsist(std::string m_name, std::string m_password);
	bool UserNameIsTaken(std::string m_name);
	User* GetUser(std::string m_name, std::string m_password);
	User* MakeNewUser(std::string m_name, std::string m_password);


	private:
	MySQL* mysql;
	User* user;
};




#endif