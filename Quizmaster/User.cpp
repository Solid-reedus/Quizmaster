#include "User.h"

User::User()
{
	name = "";
	score = 0;
}

User::User(std::string m_name, int m_score)
{
	name = m_name;
	score = m_score;
}

User::~User()
{

}