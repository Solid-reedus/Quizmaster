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

#ifndef ICON
#include "Icon.h"
#endif

#ifndef I_SEARCH_DIALOGABLE
#include "ISearchDialogable.h"
#endif

class Button : public ISearchDialogable
{
	public:
	Button();
	Button(int m_xPos, int m_yPos, int m_height, int m_width, 
		   SDL_Color m_color, SDL_Renderer* m_renderer);
	~Button();
	void OnClick(int* m_x, int* m_y);
	void Free();
	void SetText(std::string m_text, int m_size, TTF_Font* m_font, SDL_Color m_color);
	void SetIcon(SDL_Texture* m_texture = nullptr, float m_angle = NULL);
	void ChangeText(std::string m_text);
	void SetColor(SDL_Color m_color);
	void SetTextMaxWidth(int m_width);

	virtual void Render() override;
	virtual void SetNewRect(const int m_xPos, const int m_yPos, const int m_width, const int m_height) override;
	virtual void UpdateHeight(const int newHeight) override;

	Event event;

	private:
	Text* text;
	Icon* icon;
	SDL_Rect rect;
	SDL_Color color;
	SDL_Texture* buttonTexture;
	SDL_Renderer* renderer;

	void UpdateTexture();

};

#endif