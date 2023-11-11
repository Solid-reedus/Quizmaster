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

enum ShowAdminPanel
{
    NewQstn = 0,
    delQstn = 1,
    NewCat = 2,
    delCat = 3,
    admUser = 4,
    delUser = 5,
    addQstn = 6,
};

ProgramStates programState = login;
ShowAdminPanel showAdminPanel = NewQstn;

int const SCREEN_WIDTH = 1200;
int const SCREEN_HEIGHT = 800;
uint8_t const SCROLL_AMOUNT = 20;

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
Text timeTotText;
Text timeCurText;
Text difficultyText;
Text warningText;
Text nameInputText;
Text passwordInputText;
Text adminPanelPageText;

// button classes
Button loginButton;
Button registerButton;

Button loginBtn;
Button signUpBtn;

Button increaseTime;
Button decreaseTime;

Button startGameButton;

Button goToAdminButton;
Button goToSetupFromAdminButton;

Button SkipQuestionButton;
Button HalfQuestionsButton;

TextInput nameTextInput;
TextInput passwordTextInput;

SearchDialog searchDialog;


// this event is used to invoke all Onclick events
// Onclick is used to invoke all events that a button ot textInput has
Event clickEvent;

Event scrollEvent;
int scrollAmount = 0;

int difficulty[7][2] =
{
    // 5 questions - 20 sec
    {5, 20},
    {10, 30},
    {15, 45},
    {30, 60},
    {50, 75},
    {75, 90},
    {100, 120}
};

// this keeps track of what difficulty is selected
int difficultyIndex = 0;

// these values are used as positions for the clickable classes to
// check if its clicked 
int mouseX = 0;
int mouseY = 0;

int CurrentScore = 0;
int questionIndex = 0;
int questionTotal = 0;

int countdownTime = 0;
int startTime = 0;
int elapsedTime = 0;
int currentTime = 0;

int countdownWarning = 0;
int startWarning = 0;
int elapsedWarning = 0;

// this text is used to intercact with a textInput and the main event in the 
// update loop
Text* editedText = nullptr;

// this is a shortend way to generate a random number
int getRandomNumber(int m_min, int m_max)
{
    int randomNumber = m_min + std::rand() % (m_max - m_min + 1);
    return randomNumber;
}

void ShowWarning(std::string m_text, int m_seconds)
{
    warningText.NewText(m_text);

    startWarning = SDL_GetTicks() / 1000;
    countdownWarning = m_seconds;
}

// this is a switch that changes what is shown on the admin panel
void SetShowAdminPanel(int m_index)
{
    switch (m_index)
    {
    case 0:
        adminPanelPageText.NewText("new question");
        showAdminPanel = NewQstn;
        break;
    case 1:
        adminPanelPageText.NewText("delete question");
        showAdminPanel = delQstn;
        break;
    case 2:
        adminPanelPageText.NewText("new catagory");
        showAdminPanel = NewCat;
        break;
    case 3:
        adminPanelPageText.NewText("delete catagory");
        showAdminPanel = delCat;
        break;
    case 4:
        adminPanelPageText.NewText("make user admin");
        showAdminPanel = admUser;
        break;
    case 5:
        adminPanelPageText.NewText("delete user");
        showAdminPanel = delUser;
        break;
    case 6:
        adminPanelPageText.NewText("add extra ansers");
        showAdminPanel = addQstn;
        break;
    default:
        break;
    }
}

// this function is used to check, initialize and define everything
// before starting the program, if something is worng the program will
// stop with a error
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

    // this code checks if the SDL_image lib is functioning correctly
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        success = false;
    }

    // this code checks if the TTF_OpenFont lib is functioning correctly
    if (TTF_Init() == -1)
    {
        printf("TTF_OpenFont failed! TTF_Error: %s\n", TTF_GetError());
        success = false;
    }

    // defines global font 
    gFont = TTF_OpenFont("mcFont.ttf", 24);

    if (!gFont)
    {
        printf("gFont is indefined");
        success = false;
    }

    // this code will generate a random seed so the program can use random numbers
    std::srand(static_cast<unsigned int>(std::time(nullptr)));


    // here multiple things get defined
    quiz.SetMySQL(&mysql);
    acountManager.SetMySQL(&mysql);
    catagories = mysql.GetCategories();


    #pragma region catagory pos init


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

    #pragma endregion button init

    #pragma region button init

        increaseTime = Button(SCREEN_WIDTH - 625, SCREEN_HEIGHT - 175, 100, 100, { 255, 255,255 }, gRenderer);
        decreaseTime = Button(0, SCREEN_HEIGHT - 175, 100, 100, { 255, 255,255 }, gRenderer);
        increaseTime.SetIcon(loadTexture("arrow.png", gRenderer), 90);
        decreaseTime.SetIcon(loadTexture("arrow.png", gRenderer), -90);

        pageTitle = Text("welcome", SCREEN_WIDTH / 2, 15, 125, gFont, { 0,0,0 }, gRenderer, middle);
        QuestionText = Text("n/a", SCREEN_WIDTH / 2, 150, 50, gFont, { 0,0,0 }, gRenderer, middle);
        QuestionText.SetMaxWidth(SCREEN_WIDTH - 250);

        scoreText = Text("n/a", SCREEN_WIDTH - 275, 40, 75, gFont, { 0,0,0 }, gRenderer, middle);

        timeTotText = Text("tot time:", 50, 40, 45, gFont, { 0,0,0 }, gRenderer, left);
        timeCurText = Text("time:", 140, 80, 45, gFont, { 0,0,0 }, gRenderer, left);



        difficultyText = Text("n/a", 325, SCREEN_HEIGHT - 150, 40, gFont, { 0,0,0 }, gRenderer, middle);
        warningText = Text("", SCREEN_WIDTH / 2, 125, 50, gFont, { 250,50,50 }, gRenderer, middle);

        nameInputText = Text("name:", SCREEN_WIDTH / 2, 200, 50, gFont, { 0,0,0 }, gRenderer, middle);
        passwordInputText = Text("password:", SCREEN_WIDTH / 2, 350, 50, gFont, { 0,0,0 }, gRenderer, middle);

        adminPanelPageText = Text("page:", SCREEN_WIDTH / 3, 150, 50, gFont, { 0,0,0 }, gRenderer, left);

        nameTextInput = TextInput(250, 250, 650, 80, { 200,200,200 }, { 1,1,1 },
            { 10,200,10 }, gRenderer, gFont);
        nameTextInput.text->SetMaxWidth(630);
        passwordTextInput = TextInput(250, 400, 650, 80, { 200,200,200 }, { 1,1,1 },
            { 10,200,10 }, gRenderer, gFont);
        passwordTextInput.text->SetMaxWidth(630);

        loginButton = Button(250, 550, 300, 150, { 100,100,100 }, gRenderer);
        registerButton = Button(600, 550, 300, 150, { 100,100,100 }, gRenderer);
        loginButton.SetText("login", 60, gFont, { 255, 255 , 255 });
        registerButton.SetText("register", 60, gFont, { 255, 255 , 255 });

        goToAdminButton = Button(40, 40, 300, 100, { 200, 200, 200 }, gRenderer);
        goToSetupFromAdminButton = Button(40, 40, 300, 100, { 200, 200, 200 }, gRenderer);

        goToAdminButton.SetText("go to admin", 40, gFont, { 10,10,10 });
        goToSetupFromAdminButton.SetText("go to setup", 40, gFont, { 10,10,10 });

        startGameButton = Button(SCREEN_WIDTH - 450, SCREEN_HEIGHT - 200, 400, 150, { 100, 220,100 }, gRenderer);
        startGameButton.SetText("start the game", 50, gFont, { 0,0,0 });


        SkipQuestionButton = Button(350, 40, 175, 80, { 220, 60, 250 }, gRenderer);
        SkipQuestionButton.SetText("skip", 50, gFont, { 0,0,0 });

        HalfQuestionsButton = Button(550, 40, 175, 80, { 50, 50, 250 }, gRenderer);
        HalfQuestionsButton.SetText("half", 50, gFont, { 0,0,0 });

        searchDialog = SearchDialog(25, 180, 350, SCREEN_HEIGHT - 250, 100, { 180, 180, 180 }, gRenderer);

    #pragma endregion button init

    #pragma region SearchDialogs init

        const Uint8 shdItemCount = 7;
        std::string shdItems[shdItemCount]
        {
            "new question",
            "delete question",
            "new catagory",
            "delete catagory",
            "make user admin",
            "delete user",
            "add extra answers",
        };
        for (size_t i = 0; i < shdItemCount; i++)
        {
            // new items are added by adding them to the heap and pushing a 
            // ptr to the list
            Button* b = new Button(0, 200, 300, 100, { 220, 220, 220 }, gRenderer);
            b->SetText(shdItems[i], 30, gFont, { 0,0,0 });

            // on click change the panel
            b->event += std::bind(&SetShowAdminPanel, i);
            //push item to items list
            searchDialog.AddItem(b);
        }
        scrollEvent += std::bind(&SearchDialog::OnScroll, &searchDialog, &mouseX, &mouseY, &scrollAmount);


    #pragma endregion SearchDialogs init

    return success;
}


// this function will delete and clear all memory 
// this funtion is used when exiting the program
void Close()
{
    //free heap memory
    searchDialog.FreeItems();
    quiz.Free();

    SDL_FreeSurface(gSurface);
    gSurface = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    nameTextInput.Free();
    pageTitle.Free();


    loginButton.Free();
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
        else
        {
            ShowWarning(" user doesnt exsist", 5);
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
        else
        {
            ShowWarning(" user already exsist", 5);
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

    clickEvent += std::bind(&Button::OnClick, &goToAdminButton, &mouseX, &mouseY);

    goToAdminButton.event += std::bind(SetAdmin);

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
            ShowWarning("no catagories selected", 5);
            return;
        }
        if (!quiz.HasEnoughQuestions(catagories, difficulty[difficultyIndex][0]))
        {
            ShowWarning("not enough catagories selected", 5);
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

// this function will display the new question based on m_index
void NewQuestion(int m_index)
{
    answerButtons.clear();
    clickEvent.Clear();

    clickEvent += std::bind(&Button::OnClick, &SkipQuestionButton, &mouseX, &mouseY);


    int x = 0;
    int y = 0;
    int rows = std::ceil(quiz.GetQuestion(m_index)->answers.size() / 2);

    quiz.GetQuestion(m_index);

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
        answerButtons.push_back(Button(xPos, yPos, 500, height, c, gRenderer));
        answerButtons.back().SetText(a.text, height / 4, gFont, { 0,0,0 });
        answerButtons.back().SetTextMaxWidth(450);


        Uint8 mod = quiz.GetQuestion(m_index)->value;
        answerButtons.back().event += [a, mod]()
        {
            questionIndex++;
            printf("new questionIndex =  %d \n", questionIndex);
            if (a.isTrue)
            {
                printf("answer was correct \n");

                CurrentScore += mod;
                scoreText.NewText("score:" + std::to_string(CurrentScore * 10));
            }
            if (questionIndex >= questionTotal)
            {
                //send score to database
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

    SkipQuestionButton.event += [&]()
    {
        questionIndex++;
        printf("new questionIndex =  %d \n", questionIndex);
        if (questionIndex >= questionTotal)
        {
            //send score to database
            SetScoreboard();
        }
        else
        {
            NewQuestion(questionIndex);
        }
    };


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

    goToSetupFromAdminButton.event += std::bind(SetSetUp);
    clickEvent += std::bind(&Button::OnClick, &goToSetupFromAdminButton, &mouseX, &mouseY);

    std::vector<ISearchDialogable*>* items = searchDialog.GetElements();
    Uint8 step = 0;

    for (ISearchDialogable* i : *items)
    {
        Button* buttonPtr = dynamic_cast<Button*>(i);
        if (buttonPtr)
        {
            //buttonPtr->event += std::bind(&SetShowAdminPanel, step);
            clickEvent += std::bind(&Button::OnClick, buttonPtr, &mouseX, &mouseY);
            step++;
        }
    }
}


void Updatelogin()
{

    loginButton.Render();
    registerButton.Render();

    pageTitle.Render();

    nameInputText.Render();
    nameTextInput.Render();
    passwordInputText.Render();
    passwordTextInput.Render();

    // this code set the background of the program to white
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void UpdateSetUp()
{
    if (user.isAdmin)
    {
        goToAdminButton.Render();
    }

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

    SkipQuestionButton.Render();
    HalfQuestionsButton.Render();


    currentTime = SDL_GetTicks() / 1000;
    elapsedTime = currentTime - startTime;
    if (countdownTime - elapsedTime < 1)
    {
        SetScoreboard();
    }

    timeTotText.NewText("tot time:" + std::to_string(countdownTime - elapsedTime));
    timeTotText.Render();
    timeCurText.Render();

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

void UpdateAdminPanel()
{
    switch (showAdminPanel)
    {
    case NewQstn:
        break;
    case delQstn:
        break;
    case NewCat:
        break;
    case delCat:
        break;
    case admUser:
        break;
    case delUser:
        break;
    case addQstn:
        break;
    default:
        break;
    }
}

void UpdateAdmin()
{
    adminPanelPageText.Render();
    UpdateAdminPanel();
    pageTitle.Render();
    goToSetupFromAdminButton.Render();
    searchDialog.Render();
}


void Update()
{
    bool quit = false;
    bool pressedButton = false;

    Setlogin();
    SDL_Event e;

    while (!quit)
    {
        pressedButton = false;
        //Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_MOUSEWHEEL)
            {
                SDL_GetMouseState(&mouseX, &mouseY);
                //scrollwheel up
                if (e.wheel.y > 0)
                {
                    scrollAmount = SCROLL_AMOUNT;
                    scrollEvent.Invoke();
                }
                //scrollwheel down
                else if (e.wheel.y < 0)
                {
                    scrollAmount = -SCROLL_AMOUNT;
                    scrollEvent.Invoke();
                }
            }

            if (e.type == SDL_MOUSEBUTTONUP)
            {
                // this code will invoke all onclick events that are subscriped 
                // if mouseX and mouseY are both inside the button borders
                // the button will execude their own invoke
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
            // this code will add text to editedText text when it fulfills 
            // a surden criteria
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

        // clear last screen rendered
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
        case admin:
            UpdateAdmin();
            break;

        default:
            break;
        }

        int currentWarningTime = SDL_GetTicks() / 1000;
        elapsedWarning = currentWarningTime - startWarning;
        if (countdownWarning - elapsedWarning > 1)
        {
            warningText.Render();
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