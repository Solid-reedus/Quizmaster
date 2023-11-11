
#ifndef BASE_HEADER
#include "BaseHeader.h"
#endif

#ifndef I_SEARCH_DIALOGABLE
#include "ISearchDialogable.h"
#endif

#ifndef SEARCH_DIALOG
#define SEARCH_DIALOG



class SearchDialog
{	
	public:
	SearchDialog();
	SearchDialog(int m_xPos, int m_yPos, int m_width, int m_height, int m_elementHeight,
	SDL_Color m_color, SDL_Renderer* m_renderer);
	~SearchDialog();
	void AddItem(ISearchDialogable* m_item);
	void Render();
	void OnScroll(int* m_x, int* m_y, int* m_a);
	void FreeItems();
	std::vector<ISearchDialogable*>* GetElements();
	
	private:
	std::vector<ISearchDialogable*> elements;
	SDL_Rect rect;
	SDL_Color color;
	SDL_Texture* searchDialogTexture;
	SDL_Renderer* renderer;
	int relxPos, elmSize;
};


#endif
