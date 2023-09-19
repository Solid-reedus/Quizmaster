#include "Button.h"

Button::Button()
{
	rect = {};
	color = {};
	renderer = nullptr;
	buttonTexture = nullptr;
}

Button::Button(int m_xPos, int m_yPos, int m_height, int m_width, 
			   SDL_Color m_color, SDL_Renderer* m_renderer)
{
	rect = { m_xPos, m_yPos, m_height, m_width };
	color = m_color;
	renderer = m_renderer;
	buttonTexture = nullptr;
	UpdateTexture();
}

void Button::Render()
{
	SDL_RenderCopy(renderer, buttonTexture, NULL, &rect);
}

Button::~Button()
{

}

void Button::OnClick(int x, int y)
{
	if (x > rect.x && x < rect.x + rect.w &&
		y > rect.y && y < rect.y + rect.h)
	{
		printf("\n clicked on clickable");
	}
}

void Button::UpdateTexture()
{
	SDL_Surface* surf = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, 0, 0, 0, 0);
	Uint32 clr = SDL_MapRGB(surf->format, color.r, color.b, color.g);
	SDL_FillRect(surf, NULL, clr);
	buttonTexture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
}
