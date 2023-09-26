#include "UiHeader.h"
#include "BaseHeader.h"
#include "Quiz.h"
#include "AcountManager.h"
#include <cstdlib>
#include <ctime>

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
static std::vector<Button> catagoryButtons;
static std::vector<Button> answerButtons;

std::vector<Category>* catagories;

//std::vector<Category>* MySQL::GetCategories()
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

// multiple text classes
Text pageTitle;
Text QuestionText;
Text scoreText;
Text timeText;
Text difficultyText;
Text warningText;
Text nameInputText;
Text passwordInputText;

// button classes
Button loginButton;
Button registerButton;

Button loginBtn;
Button signUpBtn;

Button increaseTime;
Button decreaseTime;

Button startGameButton;

TextInput nameTextInput;
TextInput passwordTextInput;

// this event is used to invoke all Onclick
//
Event clickEvent;

int difficulty[7][2] =
{
    // 5 questions - 10 sec
    {5, 20},
    {10, 30},
    {15, 45},
    {30, 500},
    {50, 30},
    {75, 45},
    {100, 60}
};

int difficultyIndex = 0;

int mouseX = 0;
int mouseY = 0;

int CurrentScore = 0;
int questionIndex = 0;
int questionTotal = 0;

int countdownTime = 0;
int startTime = 0;
int elapsedTime = 0;
int currentTime = 0;

Text* editedText = nullptr;




void ReplaceMemoryLocation(int*& ptr1, int*& ptr2) 
{
    ptr1 = ptr2;
}

int getRandomNumber(int m_min, int m_max) 
{

    // Seed the random number generator

    // Generate the random number within the specified range
    int randomNumber = m_min + std::rand() % (m_max - m_min + 1);

    return randomNumber;
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
        gWindow = SDL_CreateWindow("quiz master", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        success = false;
    }


    if (TTF_Init() == -1) 
    {
        printf("TTF_OpenFont failed! TTF_Error: %s\n", TTF_GetError());
        success = false;
    }

    gFont = TTF_OpenFont("mcFont.ttf", 24); 

    
    if (!gFont) 
    {
        printf("gFont is indefined");
        success = false;
    }

    quiz.SetMySQL(&mysql);
    acountManager.SetMySQL(&mysql);

    catagories = mysql.GetCategories();

    // this code will generate a random seed so the program can use random numbers
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    int catagoryXPos = -170;
    int catagoryYPos = 200;

    for (size_t i = 0; i < catagories->size(); i++)
    {
        int xPos, yPos;
        if (catagoryXPos + 300 > SCREEN_WIDTH)
        {
            catagoryXPos = 50;
            catagoryYPos += 125;
        }
        else
        {
            catagoryXPos += 220;
        }
        xPos = catagoryXPos;
        yPos = catagoryYPos;

        catagoryButtons.push_back(
            Button(xPos, yPos, 200, 100, { 150, 150, 150 }, gRenderer));

    }
    for (size_t i = 0; i < catagories->size(); i++)
    {
        Button& b = catagoryButtons[i];
        b.SetText((*catagories)[i].name, 50, gFont, { 0,0,0 });

        b.event += [i, &b]() 
        {
            int index = i;
            Category& c = (*catagories)[index];
            printf("select catagory %s and index = %d \n", c.name.c_str(), index);

            if (!c.isSelected)
            {
                c.isSelected = true;
                b.SetColor({ 50,50,250 });
            }
            else
            {
                c.isSelected = false;
                b.SetColor({ 150, 150, 150 });
            }
        };
    }

    increaseTime = Button(SCREEN_WIDTH - 625, SCREEN_HEIGHT - 175, 100, 100, { 255, 255,255 }, gRenderer);
    decreaseTime = Button(0, SCREEN_HEIGHT - 175, 100, 100, { 255, 255,255 }, gRenderer);
    increaseTime.SetIcon(loadTexture("arrow.png", gRenderer), 90);
    decreaseTime.SetIcon(loadTexture("arrow.png", gRenderer), -90);

    pageTitle = Text("welcome", SCREEN_WIDTH / 2, 15, 125, gFont, {0,0,0}, gRenderer, middle);
    QuestionText = Text("n/a", SCREEN_WIDTH / 2, 150, 50, gFont, {0,0,0}, gRenderer, middle);
    QuestionText.SetMaxWidth(SCREEN_WIDTH - 250);

    scoreText = Text("n/a", SCREEN_WIDTH - 275, 40, 75, gFont, {0,0,0}, gRenderer, middle);
    timeText = Text("time:", 125, 40, 75, gFont, {0,0,0}, gRenderer, left);

    difficultyText = Text("n/a", 325, SCREEN_HEIGHT - 150, 40, gFont, {0,0,0}, gRenderer, middle);
    warningText = Text("warning:", SCREEN_WIDTH / 2, 125, 50, gFont, {250,50,50}, gRenderer, middle);

    nameInputText = Text("name:", SCREEN_WIDTH / 2, 200, 50, gFont, {0,0,0}, gRenderer, middle);
    passwordInputText = Text("password:", SCREEN_WIDTH / 2, 350, 50, gFont, {0,0,0}, gRenderer, middle);



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

    startGameButton = Button(SCREEN_WIDTH - 450, SCREEN_HEIGHT - 200 , 400, 150, { 100, 220,100 }, gRenderer);
    startGameButton.SetText("start the game", 50, gFont, { 0,0,0 });

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
    editedText = nullptr;
    programState = gameSetup;
    pageTitle.NewText("setup");
    clickEvent.Clear();

    clickEvent += std::bind(&Button::OnClick, &decreaseTime, &mouseX, &mouseY);
    clickEvent += std::bind(&Button::OnClick, &increaseTime, &mouseX, &mouseY);

    decreaseTime.event += [&]()
    {
        if (difficultyIndex > 0)
        {
            difficultyIndex--;
        }
        difficultyText.NewText("questions:" + std::to_string(difficulty[difficultyIndex][0]) + " time:" + std::to_string(difficulty[difficultyIndex][1]));
    };

    increaseTime.event += [&]()
    {
        if (difficultyIndex < 6)
        {
            difficultyIndex++;
        }
        difficultyText.NewText("questions:" + std::to_string(difficulty[difficultyIndex][0]) + " time:" + std::to_string(difficulty[difficultyIndex][1]));
    };

    difficultyText.NewText("questions:" + std::to_string(difficulty[difficultyIndex][0]) + " time:" + std::to_string(difficulty[difficultyIndex][1]));

    clickEvent += std::bind(&Button::OnClick, &startGameButton, &mouseX, &mouseY);
    startGameButton.event += [&]()
    {
        bool atleastSelected = false;
        for (Category cat : *catagories)
        {
            if (cat.isSelected)
            {
                atleastSelected = true;
            }
        }
        if (!atleastSelected)
        {
            return;
        }
        if (!quiz.HasEnoughQuestions(catagories, difficulty[difficultyIndex][0]))
        {
            printf("not enough questions to start the quiz");
            return;
        }

        std::vector<Category>& cats = *catagories;
        quiz.StartQuiz(&cats, difficulty[difficultyIndex][0]);
        SetGame();
    };

    for (size_t i = 0; i < catagoryButtons.size(); i++)
    {
        std::cout << "Button address: " << &catagoryButtons[i] << std::endl;
        clickEvent += std::bind(&Button::OnClick, &catagoryButtons[i], &mouseX, &mouseY);
    }
}


void NewQuestion(int m_index)
{
    answerButtons.clear();
    clickEvent.Clear();
    int x = 0;
    int y = 0;
    int rows = std::ceil(quiz.GetQuestion(m_index)->answers.size() / 2);

    for (Answer a : quiz.GetQuestion(m_index)->answers)
    {
        x++;

        int height = 500 / rows;

        if (x % 3 == 0)
        {
            y++;
        }

        int yPos = (SCREEN_HEIGHT - (height + 20) * y) - (height + 20);

        int xPos;
        if (x % 2 == 1)
        {
            xPos = SCREEN_WIDTH / 2 - 500;
        }
        else
        {
            xPos = SCREEN_WIDTH / 2 + 50;
        }

        int r = getRandomNumber(75, 255);
        int g = getRandomNumber(75, 255);
        int b = getRandomNumber(75, 255);

        SDL_Color c = { r,g,b };
        answerButtons.push_back(Button(xPos, yPos, 500, height, c , gRenderer));
        answerButtons.back().SetText(a.text, height / 4, gFont, { 0,0,0 });
        answerButtons.back().SetTextMaxWidth(450);

        answerButtons.back().event += [a]()
        {
            questionIndex++;
            printf("new questionIndex =  %d \n", questionIndex);
            if (a.isTrue)
            {
                printf("answer was correct \n");
                CurrentScore++;
                scoreText.NewText("score:" + std::to_string(CurrentScore * 10));
            }
            if (questionIndex >= questionTotal)
            {
                SetScoreboard();
            }
            else
            {
                NewQuestion(questionIndex);
            }
        };
    }
    for (Button btn : answerButtons)
    {
        clickEvent += std::bind(&Button::OnClick, *&btn, &mouseX, &mouseY);
    }

    QuestionText.NewText(quiz.GetQuestion(m_index)->title);
}

void SetGame()
{
    programState = game;
    clickEvent.Clear();
    scoreText.NewText("score:00");
    questionTotal = quiz.GetQuestionCount();

    std::vector<Answer> answers;

    QuestionText.NewText(quiz.GetQuestion(0)->title);
    NewQuestion(0);
    startTime = SDL_GetTicks() / 1000;
    //difficulty
    countdownTime = difficulty[difficultyIndex][1];
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

    loginButton.Render();
    registerButton.Render();


    pageTitle.Render();
    warningText.Render();

    nameInputText.Render();
    nameTextInput.Render();
    passwordInputText.Render();
    passwordTextInput.Render();

    // this code set the background of the program to white
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void UpdateSetUp()
{
    //arrowTexture.Render();
    increaseTime.Render();
    decreaseTime.Render();
    difficultyText.Render();

    pageTitle.Render();
    startGameButton.Render();

    for (Button btn : catagoryButtons)
    {
        btn.Render();
    }
}

void UpdateGame()
{
    QuestionText.Render();
    scoreText.Render();


    currentTime = SDL_GetTicks() / 1000;
    elapsedTime = currentTime - startTime;
    if (countdownTime - elapsedTime < 1)
    {
        SetScoreboard();
    }

    timeText.NewText("time:" + std::to_string(countdownTime - elapsedTime));
    timeText.Render();

    //catagoryButtons
    for (Button btn : answerButtons)
    {
        btn.Render();
    }

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
    //SetSetUp();
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
                    clickEvent.InvokeCopy();
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
                if (e.key.keysym.sym >= SDLK_a && e.key.keysym.sym <= SDLK_z)
                {
                    char pressedKey = static_cast<char>(e.key.keysym.sym);
                    tempString += pressedKey;

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