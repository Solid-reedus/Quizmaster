#include "TextInput.h"

TextInput::TextInput()
{
	rect = {};
	color = {};
	fontColor = {};
	borderColor = {};

	renderer = nullptr;
	InputTexture = nullptr;
	text = nullptr;
}

TextInput::TextInput(int m_xPos, int m_yPos, int m_height, int m_width,
	SDL_Color m_color, SDL_Color m_fontColor, SDL_Color m_BorderColor,
	SDL_Renderer* m_renderer, TTF_Font* m_font)
{
	rect = { m_xPos, m_yPos, m_height, m_width };

	color = m_color;
	fontColor = m_fontColor;
	borderColor = m_BorderColor;

	renderer = m_renderer;
	InputTexture = nullptr;
	text = new Text("test", m_xPos * 1.05f, m_yPos * 1.05f, m_height * 0.1f, m_font, m_fontColor, renderer, left);
	UpdateTexture();
}

TextInput::~TextInput()
{
	//Free();
}

void TextInput::Free()
{
	rect = {};
	color = {};
	fontColor = {};
	borderColor = {};

	renderer = nullptr;
	SDL_DestroyTexture(InputTexture);
}

void TextInput::UseExternalText(Text* m_text)
{
	text = m_text;
}

void TextInput::OnClick(int* m_x, int* m_y)
{
	if (*m_x > rect.x && *m_x < rect.x + rect.w &&
		*m_y > rect.y && *m_y < rect.y + rect.h)
	{
		printf("\n clicked on clickable");
		event.Invoke();
	}
}

void TextInput::UpdateTexture()
{
	text->NewText(textString);
	SDL_Surface* surf = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, 0, 0, 0, 0);
	Uint32 clr = SDL_MapRGB(surf->format, color.r, color.b, color.g);
	SDL_FillRect(surf, NULL, clr);
	InputTexture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
}

void TextInput::EditText(Text*& m_text)
{
	m_text = text;
}

void TextInput::Render()
{
	SDL_RenderCopy(renderer, InputTexture, NULL, &rect);
	text->Render();
}
