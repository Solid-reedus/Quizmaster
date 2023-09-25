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
	alignment = unset;
	textTexture = nullptr;
	renderer = nullptr;

}

Text::Text(std::string m_text, int m_xPos, int m_yPos, int m_size,
	TTF_Font* m_font, SDL_Color m_color, SDL_Renderer* m_renderer, Alignment m_alignment)
{
	text = m_text;
	xPos = m_xPos;
	yPos = m_yPos;
	size = m_size;
	font = m_font;
	color = m_color;
	maxWidth = NULL;
	textTexture = nullptr;
	renderer = m_renderer;
	alignment = m_alignment;
}

void Text::Render()
{
	if (!textTexture)
	{
		UpdateTexture();
	}

	int xPosition = xPos;


	int width;
	if (maxWidth == NULL || text.length() * size / 2 < maxWidth)
	{
		width = text.length() * size / 2;
	}
	else
	{
		width = maxWidth;
	}

	switch (alignment)
	{
		case middle:
			xPosition = xPos - width / 2;
			break;
		case right:
			xPosition = xPos + width;
			break;
		case left:
		default:
			break;
	}



	SDL_Rect textRect = { xPosition, yPos, width, size};

	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
}

void Text::UpdateTexture()
{
	SDL_Surface* textSurface;
	if (maxWidth == NULL)
	{
		textSurface = TTF_RenderText_Solid(font, text.c_str(), color); // Replace text and color as needed
	}
	else
	{
		textSurface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), color, maxWidth); // Replace text and color as needed
	}

    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
}

void Text::NewText(std::string m_text)
{
	printf("changed text \n");
	text = m_text;
	UpdateTexture();
}

std::string Text::GetText()
{
	return text;
}

void Text::NewColor(SDL_Color m_color)
{
	color = m_color;
	UpdateTexture();
}

void Text::SetMaxWidth(int m_width)
{
	maxWidth = m_width;
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