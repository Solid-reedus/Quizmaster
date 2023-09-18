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
}

void Button::Render()
{
	//SDL_Surface* surf = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, 0, 0, 0, 0);
	//Uint32 red = SDL_MapRGB(surf->format, 255, 0, 0);
	//SDL_FillRect(surf, NULL, red);



	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(renderer, &rect);
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

}
