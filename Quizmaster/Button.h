#ifndef BUTTON
#define BUTTON

#ifndef BASE_HEADER
#include "BaseHeader.h"
#endif

#ifndef TEXT
#include "Text.h"
#endif

#ifndef EVENT
#include "Event.h"
#endif

#ifndef CLICKABLE
#include "Iclickable.h"
#endif


class Button
{
	public:
	Button();
	Button(int m_xPos, int m_yPos, int m_height, int m_width, 
		   SDL_Color m_color, SDL_Renderer* m_renderer);
	~Button();
	void Render();
	void OnClick(int* m_x, int* m_y);

	Event event;

	private:

	SDL_Rect rect;
	SDL_Color color;
	SDL_Texture* buttonTexture;
	SDL_Renderer* renderer;

	void UpdateTexture();

};

enum ButtonFlags
{
	none = 0,
	text = 1,
	hover = 2,
	border = 4,
	all = text | hover | border
};

#endif