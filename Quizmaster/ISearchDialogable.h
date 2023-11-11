#ifndef I_SEARCH_DIALOGABLE

#define I_SEARCH_DIALOGABLE
#include "BaseHeader.h"


class ISearchDialogable
{
    public:
    virtual void UpdateHeight(const int newHeight) = 0;
    virtual void Render() = 0;
    virtual void SetNewRect(const int m_xPos, const int m_yPos, const int m_width, const int m_height) = 0;

};

#endif