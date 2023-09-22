#ifndef USER
#define USER

#include "string"

class User
{
	public:
	User();
	User(std::string m_name, int m_score);
	~User();

	std::string name;
	int score;

	private:



};

#endif
