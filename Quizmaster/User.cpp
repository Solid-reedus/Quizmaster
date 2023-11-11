#include "User.h"

User::User()
{
	name = "";
	score = 0;
}

User::User(std::string m_name, int m_id, int m_score, bool m_isAdmin)
{
	name = m_name;
	id = m_id;
	score = m_score;
	isAdmin = m_isAdmin;
}

User::~User()
{

}