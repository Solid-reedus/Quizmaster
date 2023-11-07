#include "Button.h"

Button::Button()
{
	rect = {};
	color = {};
	renderer = nullptr;
	buttonTexture = nullptr;
	text = nullptr;
	icon = nullptr;
}

Button::Button(int m_xPos, int m_yPos, int m_width, int m_height,
	SDL_Color m_color, SDL_Renderer* m_renderer)
{
	rect = { m_xPos, m_yPos, m_width, m_height };
	color = m_color;
	renderer = m_renderer;
	buttonTexture = nullptr;
	text = nullptr;
	icon = nullptr;
	UpdateTexture();
}

void Button::Render()
{
	SDL_RenderCopy(renderer, buttonTexture, NULL, &rect);
	if (text)
	{
		text->Render();
	}
	if (icon)
	{
		icon->Render();
	}
}

Button::~Button()
{
	color = {};
	rect = {};
}

void Button::Free()
{
	SDL_DestroyTexture(buttonTexture);
	delete text;
	delete icon;
	rect = {};
}

void Button::SetText(std::string m_text, int m_size, TTF_Font* m_font, SDL_Color m_color)
{
	if (icon == nullptr)
	{
		text = new Text(m_text, rect.x + rect.w / 2, rect.y + rect.h / 3, m_size, m_font, m_color, renderer, middle);
	}
	else
	{
		printf("button has already a icon, unable to set text \n");
	}
}

void Button::SetIcon(SDL_Texture* m_texture, float m_angle)
{
	if (text == nullptr)
	{
		icon = new Icon(rect.x, rect.y, rect.h, rect.w, renderer, m_angle);
		if (m_texture != nullptr)
		{
			icon->SetTexture(m_texture);
		}
	}
	else
	{
		printf("button has already a text, unable to set icon \n");
	}
}

void Button::SetTextMaxWidth(int m_width)
{
	text->SetMaxWidth(m_width);
}

void Button::ChangeText(std::string m_text)
{
	if (text != nullptr)
	{
		text->NewText(m_text);
	}
	else
	{
		printf("unable to change text, text is nullptr \n");
	}
}

void Button::OnClick(int* m_x, int* m_y)
{
	if (*m_x > rect.x && *m_x < rect.x + rect.w &&
		*m_y > rect.y && *m_y < rect.y + rect.h)
	{
		printf("clicked on clickable \n");
		event.Invoke();
	}
}

void Button::SetColor(SDL_Color m_color)
{
	color = m_color;
	UpdateTexture();
}

void Button::UpdateTexture()
{
	SDL_Surface* surf = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, 0, 0, 0, 0);
	Uint32 clr = SDL_MapRGB(surf->format, color.r, color.b, color.g);
	SDL_FillRect(surf, NULL, clr);
	buttonTexture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
}


void Button::UpdateHeight(const int newHeight)
{
	rect.y -= newHeight;
	UpdateTexture();
}

void Button::SetNewRect(const int m_xPos, const int m_yPos, const int m_width, const int m_height)
{
	rect = { m_xPos, m_yPos, m_width, m_height };
	UpdateTexture();
}