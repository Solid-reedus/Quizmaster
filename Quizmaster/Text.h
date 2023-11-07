
#ifndef BASE_HEADER
#include "BaseHeader.h"
#endif

#ifndef TEXT
#define TEXT
#include <SDL_ttf.h>

enum Alignment
{
	unset = 0,
	left = 1,
	middle = 2,
	right = 3,

};

class Text
{
	public:
	// empty constructor where verythings is null
	Text();
	// this constructor will make the class usable
	Text(std::string m_text, int m_xPos, int m_yPos, int m_size,
		 TTF_Font* m_font, SDL_Color m_color, SDL_Renderer* m_renderer, Alignment m_alignment);

	// this method will render the text
	void Render();

	void SetNewPos(int m_xPos, int m_yPos);
	void NewText(std::string m_text);
	void NewColor(SDL_Color m_color);
	std::string GetText();
	void SetMaxWidth(int m_width);

	void Free();
	~Text();

	private:

	// this method will update the texture that is used to render the text
	void UpdateTexture();

	int xPos, yPos, size, maxWidth;
	std::string text;
	TTF_Font* font;
	SDL_Color color;
	Alignment alignment;

	// the final result of the text is stored in textTexture
	SDL_Texture* textTexture;
	// the result will be rendered with a pointer renderer that points to a renderer
	// determined by the constructor
	SDL_Renderer* renderer;

};


#endif
