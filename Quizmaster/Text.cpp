#include "Text.h"

//comments in .h file

Text::Text()
{
	text = "";
	xPos = 0;
	yPos = 0;
	size = 0;
	font = 0;
	color = {0,0,0};
	textTexture = nullptr;
	renderer = nullptr;
}

Text::Text(std::string m_text, int m_xPos, int m_yPos, int m_size,
		   TTF_Font* m_font, SDL_Color m_color, SDL_Renderer* m_renderer)
{
	text = m_text;
	xPos = m_xPos;
	yPos = m_yPos;
	size = m_size;
	font = m_font;
	color = m_color;
	textTexture = nullptr;
	renderer = m_renderer;
}

void Text::Render()
{
	if (!textTexture)
	{
		UpdateTexture();
	}

	SDL_Rect textRect = { xPos, yPos, text.length() * size / 2, size};

	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
}

void Text::UpdateTexture()
{
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color); // Replace text and color as needed
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
}

void Text::NewText(std::string m_text)
{
	text = m_text;
	UpdateTexture();
}

void Text::NewColor(SDL_Color m_color)
{
	color = m_color;
	UpdateTexture();
}

void Text::Free()
{
	SDL_DestroyTexture(textTexture);
	renderer = nullptr;
	font = nullptr;
	xPos = 0;
	yPos = 0;
	size = 0;
}

Text::~Text()
{
	if (textTexture)
	{
		Free();
	}
	xPos = 0;
	yPos = 0;
	size = 0;
	font = nullptr;
	renderer = nullptr;
	textTexture = nullptr;

}

void Text::SetNewPos(int m_xPos, int m_yPos)
{
	xPos = m_xPos;
	yPos = m_yPos;
	UpdateTexture();
}