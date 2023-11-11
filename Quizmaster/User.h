#ifndef USER
#define USER

#include "string"

class User
{
	public:
	User();
	User(std::string m_name, int m_id, int m_score, bool m_isAdmin);
	~User();

	std::string name;
	int score;
	int id;
	bool isAdmin;

	private:



};

#endif
