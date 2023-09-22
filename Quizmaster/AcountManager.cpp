#include "AcountManager.h"

AcountManager::AcountManager()
{
	mysql = nullptr;
	user = nullptr;
}

AcountManager::~AcountManager()
{

}

void AcountManager::SetMySQL(MySQL* m_mysql)
{
	mysql = m_mysql;
}

bool AcountManager::UserExsist(std::string m_name, std::string m_password)
{
	if (mysql == nullptr)
	{
		printf("unable to get user because mysql ptr is null");
		return false;
	}
	
	return mysql->UserExsist(m_name, m_password);
}

bool AcountManager::UserNameIsTaken(std::string m_name)
{
	if (mysql == nullptr)
	{
		printf("unable to get user because mysql ptr is null");
		return false;
	}

	return mysql->UserNameIsTaken(m_name);
}


User* AcountManager::GetUser(std::string m_name, std::string m_password)
{
	if (mysql == nullptr)
	{
		printf("unable to get user because mysql ptr is null");
		return nullptr;
	}

	return mysql->GetUser(m_name, m_password);
}

User* AcountManager::MakeNewUser(std::string m_name, std::string m_password)
{
	if (mysql == nullptr)
	{
		printf("unable to make new user because mysql ptr is null");
		return nullptr;
	}

	mysql->MakeAcount(m_name, m_password);
	return GetUser(m_name, m_password);
}