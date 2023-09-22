#include "Category.h"

Category::Category(int m_id, std::string m_name)
{
	id = m_id;
	name = m_name;
	isSelected = false;
}

Category::~Category()
{

}