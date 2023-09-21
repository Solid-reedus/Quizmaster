#ifndef TEXT_INPUT
#define TEXT_INPUT

#ifndef BASE_HEADER
#include "BaseHeader.h"
#endif

#ifndef TEXT
#include "Text.h"
#endif

#ifndef EVENT
#include "Event.h"
#endif

class TextInput
{
	public:
	TextInput();
	TextInput(int m_xPos, int m_yPos, int m_height, int m_width, 
			  SDL_Color m_color, SDL_Color m_fontColor, SDL_Color m_BorderColor, 
			  SDL_Renderer* m_renderer, TTF_Font* m_font);
	~TextInput();

	void Render();
	void OnClick(int* m_x, int* m_y);
	void Free();
	void UseExternalText(Text* m_text);

	Event event;

	private:

	SDL_Rect rect;
	SDL_Color color;
	SDL_Color fontColor;
	SDL_Color borderColor;

	SDL_Texture* InputTexture;
	SDL_Renderer* renderer;

	Text* text;

	void UpdateTexture();

};



enum TextInputFlags
{
	noneIpt = 0,
	hoverIpt = 1,
	borderIpt = 2,
	allIpt = hoverIpt | borderIpt
};

#endif