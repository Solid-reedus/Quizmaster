#include "UiHeader.h"
#include "BaseHeader.h"
#include "Quiz.h"
#include "AcountManager.h"


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

static MySQL mysql;
static Quiz quiz;
static AcountManager acountManager;

// initialization function will initialize methods, values and ect 
// if it isnt able to initialize all of these things it will return a false
// and will close the program
bool Init();

// close function will destroy all memory that has been allocated 

void Close();
// this funtion is used to update things
void Update();


// these updates are used to update ui and other things specific to that page
void Updatelogin();
void UpdateSetUp();
void UpdateGame();
void UpdateScoreboard();
void UpdateAdmin();

// these set up functions are used to set and change events and values
// otherwise you can for example click a button from the game page in login
void Setlogin();
void SetSetUp();
void SetGame();
void SetScoreboard();
void SetAdmin();


// SDL_Window is used to determine where the content will be rendered  
SDL_Window* gWindow = nullptr;

// SDL_Renderer is used to render things on to gWindow
SDL_Renderer* gRenderer = nullptr;

//The surface contained by the window
SDL_Surface* gSurface = nullptr;

TTF_Font* gFont = nullptr;

User user;

Text pageTitle;
Button loginButton;
Button registerButton;

Button optionABtn;
Button optionBBtn;
Button optionCBtn;
Button optionDBtn;

Button loginBtn;
Button signUpBtn;
TextInput nameTextInput;
TextInput passwordTextInput;

Event testEvent;
Event clickEvent;

int mouseX = 0;
int mouseY = 0;

Text* editedText = nullptr;


void PrintStuff(std::string s)
{
    printf("I am %s \n ", s.c_str());
}

void Numbers(int a, int b)
{
    int c = a + b;
    printf("I am %d \n", c);
}

void ReplaceMemoryLocation(int*& ptr1, int*& ptr2) 
{
    ptr1 = ptr2;
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

    quiz.SetMySQL(&mysql);
    acountManager.SetMySQL(&mysql);
    
    pageTitle = Text("welcome", SCREEN_WIDTH / 2, 50, 125, gFont, {0,0,0}, gRenderer, middle);

    nameTextInput = TextInput(250, 250, 650, 80, { 200,200,200 }, { 1,1,1 },
                             { 10,200,10 }, gRenderer, gFont);
    nameTextInput.text->SetMaxWidth(630);

    passwordTextInput = TextInput(250, 400, 650, 80,{ 200,200,200 }, { 1,1,1 },
                                 { 10,200,10 }, gRenderer, gFont);
    passwordTextInput.text->SetMaxWidth(630);

    loginButton = Button(250, 550, 300, 150, { 100,100,100 }, gRenderer);
    registerButton = Button(600, 550, 300, 150, { 100,100,100 }, gRenderer);
    loginButton.SetText("login", 60, gFont, {255, 255 , 255 });
    registerButton.SetText("register", 60, gFont, {255, 255 , 255 });

    optionABtn = Button(200, 200, 300, 150, { 200, 50,50  }, gRenderer);
    optionBBtn = Button(650, 200, 300, 150, { 50, 200,50  }, gRenderer);
    optionCBtn = Button(200, 450, 300, 150, { 50, 50, 200 }, gRenderer);
    optionDBtn = Button(650, 450, 300, 150, { 200, 200,50 }, gRenderer);


    quiz.StartQuiz("1");

    return success;
}



void Close()
{
    quiz.Free();
    //Deallocate surface

    SDL_FreeSurface(gSurface);
    gSurface = NULL;

    //Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    nameTextInput.Free();
    pageTitle.Free();
    optionABtn.Free();
    optionBBtn.Free();
    optionCBtn.Free();
    optionDBtn.Free();

    TTF_CloseFont(gFont);

    IMG_Quit();
    SDL_Quit();
}


void Setlogin()
{
    programState = login;
    clickEvent.Clear();
    pageTitle.NewText("login");

    clickEvent += std::bind(&Button::OnClick, &loginButton, &mouseX, &mouseY);
    clickEvent += std::bind(&Button::OnClick, &registerButton, &mouseX, &mouseY);

    loginButton.event += [&]() 
    {
        std::string name = nameTextInput.text->GetText();
        std::string password = passwordTextInput.text->GetText();

        printf("name = %s and password = %s \n ", name.c_str(), password.c_str());
        if (acountManager.UserExsist(name, password))
        {
            printf("user exisit \n ");
            User localUser = *acountManager.GetUser(name, password);
            user = localUser;
            SetSetUp();
        }
        nameTextInput.EditText(editedText);
    };

    registerButton.event += [&]()
    {
        std::string name = nameTextInput.text->GetText();
        std::string password = passwordTextInput.text->GetText();
        if (!acountManager.UserNameIsTaken(name))
        {
            acountManager.MakeNewUser(name, password);
            User localUser = *acountManager.GetUser(name, password);
            user = localUser;
            SetSetUp();
        }
    };

    clickEvent += std::bind(&TextInput::OnClick, &nameTextInput, &mouseX, &mouseY);
    clickEvent += std::bind(&TextInput::OnClick, &passwordTextInput, &mouseX, &mouseY);

    nameTextInput.event += [&]() {nameTextInput.EditText(editedText);};
    passwordTextInput.event += [&]() {passwordTextInput.EditText(editedText);};
    
}
void SetSetUp()
{
    clickEvent.Clear();
    editedText = nullptr;
    programState = gameSetup;
    pageTitle.NewText("setup");

    //EditText
}
void SetGame()
{
    programState = game;
    clickEvent.Clear();
    pageTitle.NewText("game");
    clickEvent += std::bind(&Button::OnClick, &optionABtn, &mouseX, &mouseY);
    clickEvent += std::bind(&Button::OnClick, &optionBBtn, &mouseX, &mouseY);
    clickEvent += std::bind(&Button::OnClick, &optionCBtn, &mouseX, &mouseY);
    clickEvent += std::bind(&Button::OnClick, &optionDBtn, &mouseX, &mouseY);

}
void SetScoreboard()
{
    pageTitle.NewText("scoreboard");
    programState = scoreboard;
    clickEvent.Clear();
}
void SetAdmin()
{
    pageTitle.NewText("admin");
    programState = admin;
    clickEvent.Clear();
}



void Updatelogin()
{

    // this method will clear the last screen so you dont see 
    // things from the last rendered frame

    loginButton.Render();
    registerButton.Render();

    pageTitle.Render();

    nameTextInput.Render();
    passwordTextInput.Render();

    // this code set the background of the program to white
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void UpdateSetUp()
{
    pageTitle.Render();
}

void UpdateGame()
{
    optionABtn.Render();
    optionBBtn.Render();
    optionCBtn.Render();
    optionDBtn.Render();
    pageTitle.Render();

}

void UpdateScoreboard()
{
    pageTitle.Render();
}

void UpdateAdmin()
{
    pageTitle.Render();
}




void Update()
{
    //Main loop flag
    bool quit = false;

    bool pressedButton = false;

    Setlogin();

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
            //nameTextInput.event.Invoke();
            if (e.key.repeat == 0 && e.type == SDL_KEYDOWN && editedText != nullptr)
            {
                std::string tempString = editedText->GetText();
                printf("texting!!! \n");
                if (e.key.keysym.sym >= SDLK_a && e.key.keysym.sym <= SDLK_z)
                {
                    char pressedKey = static_cast<char>(e.key.keysym.sym);
                    tempString += pressedKey;

                    PrintStuff(tempString);
                    printf("test \n");

                    editedText->NewText(tempString);
                }
                else if (e.key.keysym.sym == SDLK_SPACE)
                {
                    tempString += " ";
                    editedText->NewText(tempString);
                }
                else if (e.key.keysym.sym == SDLK_BACKSPACE)
                {
                    if (tempString.size() > 0)
                    {
                        tempString.erase(tempString.end() - 1, tempString.end());
                        editedText->NewText(tempString);
                    }
                }
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
    Close();
	return 0;
}