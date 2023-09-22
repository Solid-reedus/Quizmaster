#ifndef CATEGORY
#define CATEGORY

#ifndef BASE_HEADER
#include "BaseHeader.h"
#endif

class Category
{
	public:
	Category(int m_id, std::string m_name);
	~Category();
	bool isSelected;
	int id;
	std::string name;

	private:

};

#endif
