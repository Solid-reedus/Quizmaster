#include "UiHeader.h"
#include "BaseHeader.h"
#include "Quiz.h"
#include "Event.h"


enum ProgramStates
{
    login = 1,
    gameSetup = 2,
    game = 3,
    scoreboard = 4,
    admin = 5

};

ProgramStates programState = login;

int const SCREEN_WIDTH = 1200;
int const SCREEN_HEIGHT = 800;
static Quiz quiz;


// initialization function will initialize methods, values and ect 
// if it isnt able to initialize all of these things it will return a false
// and will close the program
bool Init();

// close function will destroy all memory that has been allocated 
void Close();

// this funtion is used to update things
void Update();

void Updatelogin();
void UpdateSetUp();
void UpdateGame();
void UpdateScoreboard();
void UpdateAdmin();

// SDL_Window is used to determine where the content will be rendered  
SDL_Window* gWindow = nullptr;

// SDL_Renderer is used to render things on to gWindow
SDL_Renderer* gRenderer = nullptr;

//The surface contained by the window
SDL_Surface* gSurface = nullptr;

TTF_Font* gFont = nullptr;

Text testingText;
Text inputText;
Button testButton;

Button optionABtn;
Button optionBBtn;
Button optionCBtn;
Button optionDBtn;

Button loginBtn;
Button signUpBtn;

Event testEvent;
Event clickEvent;

int mouseX = 0;
int mouseY = 0;



std::string inputString = "text: ";


void PrintStuff()
{
    printf("I am stuff \n");
}

void PrintStuff2(const char* s)
{
    printf("I am %s \n ", s);
}

void Numbers(int a, int b)
{
    int c = a + b;
    printf("I am %d \n", c);
}


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

    optionABtn = Button(200, 200, 300, 150, { 200, 50,50  }, gRenderer);
    optionBBtn = Button(650, 200, 300, 150, { 50, 200,50  }, gRenderer);
    optionCBtn = Button(200, 450, 300, 150, { 50, 50, 200 }, gRenderer);
    optionDBtn = Button(650, 450, 300, 150, { 200, 200,50 }, gRenderer);

    optionABtn.OnClick(&mouseX, &mouseY);

    clickEvent += std::bind(&Button::OnClick, &optionABtn, &mouseX, &mouseY);
    clickEvent += std::bind(&Button::OnClick, &optionBBtn, &mouseX, &mouseY);
    clickEvent += std::bind(&Button::OnClick, &optionCBtn, &mouseX, &mouseY);
    clickEvent += std::bind(&Button::OnClick, &optionDBtn, &mouseX, &mouseY);

    clickEvent += std::bind(&Button::OnClick, &testButton, &mouseX, &mouseY);
    testButton.event += [&]() {programState = ProgramStates::gameSetup;};

    quiz.StartQuiz("1");

    testEvent += std::bind(Numbers, 1, 1);
    testEvent += PrintStuff;
    testEvent += std::bind(PrintStuff2, "stuu");

    testEvent.Invoke();

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

void Updatelogin()
{
    SDL_Rect textRect = { 10, 10, 100, 100 };

    // this method will clear the last screen so you dont see 
    // things from the last rendered frame
    

    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
    SDL_Rect rect = { 100, 100, 200, 200 };
    SDL_RenderDrawRect(gRenderer, &rect);

    testButton.Render();
    testingText.Render();
    inputText.Render();

    // this code set the background of the program to white
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void UpdateSetUp()
{

}

void UpdateGame()
{
    optionABtn.Render();
    optionBBtn.Render();
    optionCBtn.Render();
    optionDBtn.Render();


}

void UpdateScoreboard()
{

}

void UpdateAdmin()
{

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
            if (e.type == SDL_MOUSEBUTTONUP)
            {
                SDL_GetMouseState(&mouseX, &mouseY);
                if (mouseX > 0 && mouseX < SCREEN_WIDTH &&
                    mouseY > 0 && mouseY < SCREEN_HEIGHT)
                {
                    clickEvent.Invoke();
                }
            }
            //User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            if (e.key.keysym.sym >= SDLK_a && e.key.keysym.sym <= SDLK_z) 
            {
                char pressedKey = static_cast<char>(e.key.keysym.sym);
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

        SDL_RenderClear(gRenderer);

        switch (programState)
        {
            case login:
                Updatelogin();
                break;
            case gameSetup:
                UpdateSetUp();
                break;
            case game:
                UpdateGame();
                break;
            case scoreboard:
                UpdateScoreboard();
                break;
                UpdateAdmin();
            case admin:
                break;

            default:
                break;
        }
        
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