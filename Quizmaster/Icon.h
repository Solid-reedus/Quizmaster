#ifndef ICON
#define ICON
	
#ifndef BASE_HEADER
#include "BaseHeader.h"
#endif

SDL_Texture* loadTexture(std::string m_path, SDL_Renderer* m_renderer);

class Icon
{
	public:
	Icon();	
	Icon(int m_xPos, int m_yPos, int m_height, int m_width, SDL_Renderer* m_renderer, float m_angle = NULL);
	~Icon();

	void loadIconTexture(std::string m_path);
	void SetTexture(SDL_Texture* m_texture);
	void Render();
	void RotateTexture(float m_angle);

	private:
	SDL_Texture* texture;
	SDL_Renderer* renderer;
	SDL_Rect rect;
	float angle;

};


#endif