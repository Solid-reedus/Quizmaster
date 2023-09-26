#include "Icon.h";


Icon::Icon()
{
    rect = {};
    renderer = nullptr;
    texture = nullptr;
    angle = NULL;
}

Icon::Icon(int m_xPos, int m_yPos, int m_height, int m_width, SDL_Renderer* m_renderer, float m_angle)
{
    rect = { m_xPos, m_yPos, m_width, m_height };
    renderer = m_renderer;
    texture = nullptr;
    angle = m_angle;
}

Icon::~Icon()
{
    //delete texture;
}

void Icon::loadIconTexture(std::string m_path)
{
    texture = loadTexture(m_path, renderer);
}

void Icon::SetTexture(SDL_Texture* m_texture)
{
    texture = m_texture;
}

void Icon::RotateTexture(float m_angle)
{
    angle = m_angle;
}

void Icon::Render()
{
    if (texture == nullptr)
    {
        printf("icon hasnt set texture yet");
        return;
    }

    if (angle)
    {
        SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
    }
    else
    {
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }
}

SDL_Texture* loadTexture(std::string m_path, SDL_Renderer* m_renderer)
{
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(m_path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", m_path.c_str(), IMG_GetError());
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(m_renderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", m_path.c_str(), SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}