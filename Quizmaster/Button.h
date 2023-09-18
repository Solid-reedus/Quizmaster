#ifndef BUTTON
#define BUTTON

#ifndef BASE_HEADER
#include "BaseHeader.h"
#endif

#ifndef TEXT
#include "Text.h"
#endif

#ifndef CLICKABLE
#include "Iclickable.h"
#endif




class Button : public Clickable
{
	public:
	Button();
	Button(int m_xPos, int m_yPos, int m_height, int m_width, 
		   SDL_Color m_color, SDL_Renderer* m_renderer);
	~Button();
	void Render();
	void OnClick(int x, int y) override;

	private:

	SDL_Rect rect;
	SDL_Color color;
	SDL_Texture* buttonTexture;
	SDL_Renderer* renderer;

	void UpdateTexture();

};



#endif