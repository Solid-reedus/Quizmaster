#include "UiHeader.h"
#include "BaseHeader.h"
#include "Quiz.h"

//#include "MySQL.h"


static Quiz quiz;
//static MySQL mysql;


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

Text testingText;

std::string inputString = "text: ";
Text inputText;

Button testButton;


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
    
    
    testingText = Text("Andrzej Betiuk", 100, 100, 100, gFont, {0,0,0}, gRenderer);
    inputText = Text(inputString, 200, 400, 50, gFont, {0,0,0}, gRenderer);
    testButton = Button(200, 500, 400, 200, { 100,100,100 }, gRenderer);

    quiz.StartQuiz("1");



    return success;
}


void Close()
{
    quiz.Free();
    testingText.Free();
    inputText.Free();
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

    bool pressedButton = false;

    //Event handler
    SDL_Event e;

    //While application is running
    while (!quit)
    {
        pressedButton = false;
        //Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            //User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            if (e.key.keysym.sym >= SDLK_a && e.key.keysym.sym <= SDLK_z) 
            {
                char pressedKey = static_cast<char>(e.key.keysym.sym);
                //std::cout << "Key pressed: " << pressedKey << std::endl;
                inputString += pressedKey;
                inputText.NewText(inputString);

            }
            else if (e.key.keysym.sym == SDLK_SPACE)
            {
                inputString += " ";
                inputText.NewText(inputString);
            }
            else if (e.key.keysym.sym == SDLK_BACKSPACE)
            {
                inputString.erase(inputString.end() - 1, inputString.end());
                inputText.NewText(inputString);
            }
        }

        SDL_Rect textRect = { 10, 10, 100, 100 };

        // this method will clear the last screen so you dont see 
        // things from the last rendered frame
        SDL_RenderClear(gRenderer);



        SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
        SDL_Rect rect = { 100, 100, 200, 200 };
        SDL_RenderDrawRect(gRenderer, &rect);


        testButton.Render();
        testingText.Render();
        inputText.Render();

        // this code set the background of the program to white
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        //this code will display the final result
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