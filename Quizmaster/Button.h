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


class Button
{
	public:
	Button();
	Button(int m_xPos, int m_yPos, int m_height, int m_width, 
		   SDL_Color m_color, SDL_Renderer* m_renderer);
	~Button();
	void Render();
	void OnClick(int* m_x, int* m_y);
	void Free();
	void SetText(std::string m_text, int m_size, TTF_Font* m_font, SDL_Color m_color);
	void ChangeText(std::string m_text);
	void SetColor(SDL_Color m_color);

	Event event;

	/*
	std::string m_text, int m_xPos, int m_yPos, int m_size,
		 TTF_Font* m_font, SDL_Color m_color
	*/

	private:

	Text* text;
	SDL_Rect rect;
	SDL_Color color;
	SDL_Texture* buttonTexture;
	SDL_Renderer* renderer;

	void UpdateTexture();

};

enum ButtonFlags
{
	noneBtn = 0,
	textBtn = 1,
	hoverBtn = 2,
	borderBtn = 4,
	allBtn = textBtn | hoverBtn | borderBtn
};

#endif