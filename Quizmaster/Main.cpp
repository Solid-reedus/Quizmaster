#include <iostream>
#include "SDL.h"
#include <SDL_image.h>
#include <SDL_ttf.h>

int const SCREEN_WIDTH = 1200;
int const SCREEN_HEIGHT = 800;


// initialization function will initialize methods, values and ect 
// if it isnt able to initialize all of these things it will return a false
// and will close the program
bool Init();

// close function will destroy all memory that has been allocated 
void Close();

// this funtion is used to update things
void Update();

// SDL_Window is used to determine where the content will be rendered  
SDL_Window* gWindow = NULL;

// SDL_Renderer is used to render things on to gWindow
SDL_Renderer* gRenderer = NULL;

//The surface contained by the window
SDL_Surface* gSurface = NULL;

TTF_Font* gFont;


bool Init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            gSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    if (TTF_Init() == -1) 
    {
        printf("TTF_OpenFont failed! TTF_Error: %s\n", TTF_GetError());
        success = false;
    }

    gFont = TTF_OpenFont("mcFont.ttf", 36); 

    
    if (!gFont) 
    {
        printf("gFont is indefined");
        success = false;
    }
    


    return success;
}


void Close()
{
    //Deallocate surface
    SDL_FreeSurface(gSurface);
    gSurface = NULL;

    //Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    TTF_CloseFont(gFont);

    SDL_Quit();
    IMG_Quit();
}

void Update()
{
    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;


    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, "jeff", { 0, 0, 0 }); // Replace text and color as needed
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);

    //While application is running
    while (!quit)
    {
        //Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            //User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        SDL_Rect textRect = { 10, 10, 100, 100 };

        //Clear screen
        //SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);


        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, textTexture, NULL, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_RenderPresent(gRenderer);

    }
}


int main(int argc, char* args[])
{

    // try to initialize otherwise close the program
    if (!Init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        Update();
    }

	return 0;
}