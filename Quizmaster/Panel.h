#ifndef PANEL
#define PANEL

#ifndef BASE_HEADER
#include "BaseHeader.h"
#endif


class Panel
{
	public:
	Panel();
	Panel(int m_xPos, int m_yPos, int m_height, int m_width,
		SDL_Color m_color, SDL_Renderer* m_renderer);
	~Panel();
	void Free();
	void SetColor(SDL_Color m_color);
	void Render();

	private:
	SDL_Rect rect;
	SDL_Color color;
	SDL_Texture* panelTexture;
	SDL_Renderer* renderer;
	void UpdateTexture();
};

#endif