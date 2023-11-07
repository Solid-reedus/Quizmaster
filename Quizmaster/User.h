#ifndef USER
#define USER

#include "string"

class User
{
	public:
	User();
	User(std::string m_name, int m_score, bool m_isAdmin);
	~User();

	std::string name;
	int score;
	bool isAdmin;

	private:



};

#endif
