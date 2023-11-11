#include "Panel.h"

Panel::Panel()
{
	rect = {};
	color = {};
	renderer = nullptr;
	panelTexture = nullptr;
}

Panel::Panel(int m_xPos, int m_yPos, int m_height, int m_width,
	SDL_Color m_color, SDL_Renderer* m_renderer)
{
	rect = { m_xPos, m_yPos, m_width, m_height };
	color = m_color;
	renderer = m_renderer;
	panelTexture = nullptr;
	UpdateTexture();
}

Panel::~Panel()
{

}

void Panel::Free()
{
	SDL_DestroyTexture(panelTexture);
}

void Panel::SetColor(SDL_Color m_color)
{
	color = m_color;
	UpdateTexture();
}

void Panel::UpdateTexture()
{
	SDL_Surface* surf = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, 0, 0, 0, 0);
	Uint32 clr = SDL_MapRGB(surf->format, color.r, color.b, color.g);
	SDL_FillRect(surf, NULL, clr);
	panelTexture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
}

void Panel::Render()
{
	SDL_RenderCopy(renderer, panelTexture, NULL, &rect);
}