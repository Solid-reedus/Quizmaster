#include "UiHeader.h"
#include "BaseHeader.h"
#include "Quiz.h"
#include "AcountManager.h"
#include <cstdlib>
#include <ctime>

// this enum is used to display right page
enum ProgramStates
{
    login = 1,
    gameSetup = 2,
    game = 3,
    scoreboard = 4,
    admin = 5
};

// this is for showing the correct panel in the admin
enum ShowAdminPanel
{
    NewQstn = 0,
    delQstn = 1,
    NewCat = 2,
    delCat = 3,
    admUser = 4,
    delUser = 5,
};


ProgramStates programState = login;
ShowAdminPanel showAdminPanel = NewQstn;
GameMode currentGameMode = gmNone;

int const SCREEN_WIDTH = 1200;
int const SCREEN_HEIGHT = 800;
uint8_t const SCROLL_AMOUNT = 20;

static MySQL mysql;
static Quiz quiz;
static AcountManager acountManager;
static std::vector<Button> catagoryButtons;
static std::vector<Button> answerButtons;

static std::vector<Text> scoresScoreTexts;
static std::vector<Text> scoresNameTexts;
static std::vector<Text> scoresQuestionCountTexts;
static std::vector<Text> scoresTimeTexts;

std::vector<Category>* catagories;
std::vector<Score> scores;
std::vector<Panel> scoreBoardPanels;

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

Text scoreboardScoreText;
Text scoreboardNameText;
Text scoreboardQuestionCountText;
Text scoreboardTimeText;

// button classes
Button loginButton;
Button registerButton;

Button loginBtn;
Button signUpBtn;

Button increaseTime;
Button decreaseTime;

Button startTimedGameButton;
Button startSpeedRunGameButton;

Button goToSetupFromScoreboardButton;
Button goToAdminButton;
Button goToSetupFromAdminButton;
Button executeAdminButton;

Button SkipQuestionButton;
Button HalfQuestionsButton;

Button AddFalseQuestionButton;
Button AddTrueQuestionButton;

TextInput nameTextInput;
TextInput passwordTextInput;
TextInput adminTextInput;
TextInput adminAddQuestionTextInput;
TextInput adminCatagoryQuestionTextInput;


SearchDialog adminSearchDialog;
SearchDialog adminSubSearchDialog;
Panel AdminBackPanel;


// this event is used to invoke all Onclick events
// Onclick is used to invoke all events that a button ot textInput has
Event clickEvent;

Event scrollEvent;
int scrollAmount = 0;

int difficulty[7][2] =
{
    // 5 questions - 20 sec
    {5, 30},
    {10, 45},
    {15, 60},
    {30, 75},
    {50, 90},
    {75, 120},
    {100, 150}
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

int countdownCurQtnTime = 10;
int startCurQtnTime = 0;
int elapsedCurQtnTime = 0;
int currentCurQtnTime = 0;


int countdownWarning = 0;
int startWarning = 0;
int elapsedWarning = 0;
bool currentHalfed = false;

// this text is used to intercact with a textInput and the main event in the 
// update loop
Text* editedText = nullptr;

// these values are for the admin
// these values will change based on the panel and button pressed
std::string SelectedString;
Question currentEditedQuestion = Question();
std::vector<Answer> currentEditedAnswers;


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
    clickEvent.Clear();
    clickEvent += std::bind(&Button::OnClick, &goToSetupFromAdminButton, &mouseX, &mouseY);

    std::vector<ISearchDialogable*>* items = adminSearchDialog.GetElements();
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


    executeAdminButton.event += [m_index]()
    {

        switch (m_index)
        {
        case 0:
        {
            if (currentEditedAnswers.size() > 0 && 
                adminTextInput.text->GetText() != "" && 
                adminCatagoryQuestionTextInput.text->GetText() != "")
            {
                currentEditedQuestion = Question(adminTextInput.text->GetText(), currentEditedAnswers);
                mysql.MakeQuestion(currentEditedQuestion, adminCatagoryQuestionTextInput.text->GetText());
            }
            break;
        }
        case 1:
        {
            if (SelectedString != "")
            {
                mysql.DeleteRowOf(tblQuestions, SelectedString);
                SetShowAdminPanel(0);
            }
            break;
        }
        case 2:
        {
            if (adminTextInput.text->GetText() != "" || adminTextInput.text != nullptr)
            {
                mysql.MakeCategory(adminTextInput.text->GetText());
                adminTextInput.text->NewText("");
                SetShowAdminPanel(0);
            }
            break;
        }
        case 3:
        {
            if (SelectedString != "")
            {
                mysql.DeleteRowOf(tblCategories, SelectedString);
                SetShowAdminPanel(0);
            }
            break;
        }
        case 4:
        {
            if (SelectedString != "")
            {
                mysql.MakeAdmin(SelectedString);
                SetShowAdminPanel(0);
            }
            break;
        }
        case 5:
        {
            if (SelectedString != "")
            {
                mysql.DeleteRowOf(tblUserNames, SelectedString);
                SetShowAdminPanel(0);
            }
            break;
        }
        default:
            break;
        }
    };

    clickEvent += std::bind(&Button::OnClick, &executeAdminButton, &mouseX, &mouseY);

    adminSubSearchDialog.FreeItems();
    adminSubSearchDialog.ResetRelXPos();



    switch (m_index)
    {
    case 0:
    {
        adminPanelPageText.NewText("new question");
        adminTextInput.event += [&]() {adminTextInput.EditText(editedText);};
        adminAddQuestionTextInput.event += [&]() {adminAddQuestionTextInput.EditText(editedText);};
        adminCatagoryQuestionTextInput.event += [&]() {adminCatagoryQuestionTextInput.EditText(editedText);};

        AddFalseQuestionButton.event += []()
        {
            if (adminAddQuestionTextInput.text->GetText() == "")
            {
                return;
            }
            currentEditedAnswers.push_back(Answer(adminAddQuestionTextInput.text->GetText(), false));
            adminAddQuestionTextInput.text->NewText("");

            adminSubSearchDialog.FreeItems();
            for (const Answer a : currentEditedAnswers)
            {
                Button* b = new Button(0, 150, 200, 60, { 220, 220, 220 }, gRenderer);

                b->SetText(a.text , 60, gFont, { 0,0,0 });
                b->TextSetMaxWidth(325);
                adminSubSearchDialog.AddItem(b);
            }
        };
        AddTrueQuestionButton.event += []()
        {
            if (adminAddQuestionTextInput.text->GetText() == "")
            {
                return;
            }
            currentEditedAnswers.push_back(Answer(adminAddQuestionTextInput.text->GetText(), true));
            adminAddQuestionTextInput.text->NewText("");

            adminSubSearchDialog.FreeItems();
            for (const Answer a : currentEditedAnswers)
            {
                Button* b = new Button(0, 150, 200, 60, { 220, 220, 220 }, gRenderer);

                b->SetText(a.text, 60, gFont, { 0,0,0 });
                b->TextSetMaxWidth(325);
                adminSubSearchDialog.AddItem(b);
            }
        };

        clickEvent += std::bind(&Button::OnClick, &AddFalseQuestionButton, &mouseX, &mouseY);
        clickEvent += std::bind(&Button::OnClick, &AddTrueQuestionButton, &mouseX, &mouseY);
        clickEvent += std::bind(&TextInput::OnClick, &adminTextInput, &mouseX, &mouseY);
        clickEvent += std::bind(&TextInput::OnClick, &adminAddQuestionTextInput, &mouseX, &mouseY);
        clickEvent += std::bind(&TextInput::OnClick, &adminCatagoryQuestionTextInput, &mouseX, &mouseY);

        currentEditedQuestion = Question();

        //currentEditedAnswers

        showAdminPanel = NewQstn;
        break;
    }
    case 1:
    {
        adminPanelPageText.NewText("delete question");
        
        for (std::string r : mysql.GetAllRowsOf(tblQuestions))
        {
            Button* b = new Button(0, 150, 200, 60, { 220, 220, 220 }, gRenderer);

            b->SetText(r, 60, gFont, { 0,0,0 });
            b->TextSetMaxWidth(325);
            adminSubSearchDialog.AddItem(b);

            b->event += [r]()
            {
                SelectedString = r;
                executeAdminButton.ChangeText("delete: " + SelectedString);
            };
            clickEvent += std::bind(&Button::OnClick, *&b, &mouseX, &mouseY);

        }
        showAdminPanel = delQstn;
        break;
    }

    case 2:
    {
        adminPanelPageText.NewText("new catagory");

        clickEvent += std::bind(&TextInput::OnClick, &adminTextInput, &mouseX, &mouseY);
        adminTextInput.event += [&]() {adminTextInput.EditText(editedText);};
        executeAdminButton.ChangeText("make");

        showAdminPanel = NewCat;
        break;
    }
    case 3:
    {
        adminPanelPageText.NewText("delete catagory");

        for (std::string r : mysql.GetAllRowsOf(tblCategories))
        {
            Button* b = new Button(0, 150, 200, 60, { 220, 220, 220 }, gRenderer);

            b->SetText(r, 60, gFont, { 0,0,0 });
            b->TextSetMaxWidth(325);
            b->event += [r]()
            {
                SelectedString = r;
                executeAdminButton.ChangeText("delete: " + SelectedString);
            };
            adminSubSearchDialog.AddItem(b);
            clickEvent += std::bind(&Button::OnClick, *&b, &mouseX, &mouseY);
        }

        showAdminPanel = delCat;
        break;
    }
    case 4:
    {
        adminPanelPageText.NewText("make user admin");

        for (std::string r : mysql.GetAllRowsOf(tblUserNames))
        {
            Button* b = new Button(0, 150, 200, 60, { 220, 220, 220 }, gRenderer);

            b->SetText(r, 60, gFont, { 0,0,0 });
            b->TextSetMaxWidth(325);
            b->event += [r]()
            {
                SelectedString = r;
                executeAdminButton.ChangeText("make user: " + SelectedString + " admin");
            };
            adminSubSearchDialog.AddItem(b);
            clickEvent += std::bind(&Button::OnClick, *&b, &mouseX, &mouseY);
        }

        showAdminPanel = admUser;
        break;
    }
    case 5:
    {
        adminPanelPageText.NewText("delete user");

        for (std::string r : mysql.GetAllRowsOf(tblUserNames))
        {
            Button* b = new Button(0, 150, 200, 60, { 220, 220, 220 }, gRenderer);

            b->SetText(r, 60, gFont, { 0,0,0 });
            b->TextSetMaxWidth(325);
            b->event += [r]()
            {
                SelectedString = r;
                executeAdminButton.ChangeText("delete: " + SelectedString);
            };
            adminSubSearchDialog.AddItem(b);
            clickEvent += std::bind(&Button::OnClick, *&b, &mouseX, &mouseY);
        }

        showAdminPanel = delUser;
        break;
    }
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

        timeCurText = Text("time:", 140, 80, 45, gFont, { 0,0,0 }, gRenderer, left);

        scoreboardScoreText = Text("score", 190, 150, 45, gFont, { 0,0,0 }, gRenderer, left);
        scoreboardNameText = Text("name", 400, 150, 45, gFont, { 0,0,0 }, gRenderer, left);
        scoreboardQuestionCountText = Text("questions", 600, 150, 45, gFont, { 0,0,0 }, gRenderer, left);
        scoreboardTimeText = Text("time", 880, 150, 45, gFont, { 0,0,0 }, gRenderer, left);

        difficultyText = Text("n/a", 325, SCREEN_HEIGHT - 150, 40, gFont, { 0,0,0 }, gRenderer, middle);
        warningText = Text("", SCREEN_WIDTH / 2, 125, 50, gFont, { 250,50,50 }, gRenderer, middle);

        nameInputText = Text("name:", SCREEN_WIDTH / 2, 200, 50, gFont, { 0,0,0 }, gRenderer, middle);
        passwordInputText = Text("password:", SCREEN_WIDTH / 2, 350, 50, gFont, { 0,0,0 }, gRenderer, middle);

        adminPanelPageText = Text("page:", SCREEN_WIDTH / 3 + 50, 200, 50, gFont, { 0,0,0 }, gRenderer, left);

        nameTextInput = TextInput(250, 250, 80, 650, { 200,200,200 }, { 1,1,1 },
            { 10,200,10 }, gRenderer, gFont);
        nameTextInput.text->SetMaxWidth(630);

        passwordTextInput = TextInput(250, 400, 80, 650, { 200,200,200 }, { 1,1,1 },
            { 10,200,10 }, gRenderer, gFont);
        passwordTextInput.text->SetMaxWidth(630);

        adminTextInput = TextInput(785, SCREEN_HEIGHT - 400, 100, 300, { 150,150,150 }, { 1,1,1 },
            { 10,200,10 }, gRenderer, gFont);
        adminTextInput.text->SetMaxWidth(250);

        adminAddQuestionTextInput = TextInput(785, SCREEN_HEIGHT - 530, 50, 300, { 150,150,150 }, { 1,1,1 },
                { 10,200,10 }, gRenderer, gFont);
        adminAddQuestionTextInput.text->SetMaxWidth(250);

        adminCatagoryQuestionTextInput = TextInput(785, SCREEN_HEIGHT - 585, 50, 300, { 150,150,150 }, { 1,1,1 },
            { 10,200,10 }, gRenderer, gFont);
        adminCatagoryQuestionTextInput.text->SetMaxWidth(250);

        goToSetupFromScoreboardButton = Button(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT - 125, 300, 75, { 100,100,100 }, gRenderer);
        goToSetupFromScoreboardButton.SetText("go to setup", 40, gFont, { 10,10,10 });

        AddFalseQuestionButton = Button(785, SCREEN_HEIGHT - 470, 140, 60, { 200,50, 50 }, gRenderer);
        AddTrueQuestionButton  = Button(945, SCREEN_HEIGHT - 470, 140, 60, { 50,50, 200 }, gRenderer);
        AddTrueQuestionButton.SetText("add correct", 20, gFont, { 10,10,10 });
        AddFalseQuestionButton.SetText("add falsse", 20, gFont, { 10,10,10 });

        loginButton = Button(250, 550, 300, 150, { 100,100,100 }, gRenderer);
        registerButton = Button(600, 550, 300, 150, { 100,100,100 }, gRenderer);
        loginButton.SetText("login", 60, gFont, { 255, 255 , 255 });
        registerButton.SetText("register", 60, gFont, { 255, 255 , 255 });

        goToAdminButton = Button(40, 40, 300, 100, { 200, 200, 200 }, gRenderer);
        goToSetupFromAdminButton = Button(40, 40, 300, 100, { 200, 200, 200 }, gRenderer);
        executeAdminButton = Button(785, SCREEN_HEIGHT - 290, 300, 200, { 50, 50, 200 }, gRenderer);

        goToAdminButton.SetText("go to admin", 40, gFont, { 10,10,10 });
        goToSetupFromAdminButton.SetText("go to setup", 40, gFont, { 10,10,10 });
        executeAdminButton.SetText("none", 100, gFont, { 10,10,10 });
        executeAdminButton.TextSetMaxWidth(300);

        startTimedGameButton = Button(SCREEN_WIDTH - 450, SCREEN_HEIGHT - 240, 400, 100, { 100, 220,100 }, gRenderer);
        startTimedGameButton.SetText("start timed game", 40, gFont, { 0,0,0 });

        startSpeedRunGameButton = Button(SCREEN_WIDTH - 450, SCREEN_HEIGHT - 120, 400, 100, { 220, 100,100 }, gRenderer);
        startSpeedRunGameButton.SetText("start speedrun game", 40, gFont, { 0,0,0 });


        SkipQuestionButton = Button(350, 40, 175, 80, { 220, 60, 250 }, gRenderer);
        SkipQuestionButton.SetText("skip", 50, gFont, { 0,0,0 });

        HalfQuestionsButton = Button(550, 40, 175, 80, { 50, 50, 250 }, gRenderer);
        HalfQuestionsButton.SetText("half", 50, gFont, { 0,0,0 });

        adminSearchDialog = SearchDialog(25, 180, 350, SCREEN_HEIGHT - 250, 100, { 180, 180, 180 }, gRenderer);
        adminSubSearchDialog = SearchDialog(420, 250, 350, SCREEN_HEIGHT - 340, 100, { 150, 150, 150 }, gRenderer);

        AdminBackPanel = Panel(400, 180, SCREEN_HEIGHT - 250, SCREEN_WIDTH - 500, {200,200 ,200 }, gRenderer);

    #pragma endregion button init

    #pragma region adminSearchDialogs init

        const Uint8 shdItemCount = 6;
        std::string shdItems[shdItemCount]
        {
            "new question",
            "delete question",
            "new catagory",
            "delete catagory",
            "make user admin",
            "delete user",
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
            adminSearchDialog.AddItem(b);
        }

        scrollEvent += std::bind(&SearchDialog::OnScroll, &adminSearchDialog, &mouseX, &mouseY, &scrollAmount);
        scrollEvent += std::bind(&SearchDialog::OnScroll, &adminSubSearchDialog, &mouseX, &mouseY, &scrollAmount);


    #pragma endregion adminSearchDialogs init

    //scoreBoardPanels
    scoreBoardPanels.push_back(Panel(180, 200, 50, 820, { 255,0,215 }, gRenderer));
    scoreBoardPanels.push_back(Panel(180, 250, 50, 820, { 230,230,230 }, gRenderer));
    scoreBoardPanels.push_back(Panel(180, 300, 50, 820, { 150,90,120 }, gRenderer));
    for (size_t i = 3; i < 9; i++)
    {
        int yPos = 200 + (i * 50);

        if (i % 2 == 0)
        {
            scoreBoardPanels.push_back(Panel(180, yPos, 50, 820, { 200,200,200 }, gRenderer));
        }
        else
        {
            scoreBoardPanels.push_back(Panel(180, yPos, 50, 820, { 150,150,150 }, gRenderer));
        }
    }


    return success;
}


// this function will delete and clear all memory 
// this funtion is used when exiting the program
void Close()
{
    //free heap memory
    adminSearchDialog.FreeItems();
    quiz.Free();

    SDL_FreeSurface(gSurface);
    gSurface = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    nameTextInput.Free();
    passwordInputText.Free();
    adminTextInput.Free();
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

        if (name == "")
        {
            ShowWarning("name is empty", 3);
        }
        else if (password == "")
        {
            ShowWarning("password is empty", 3);
        }
        else if (acountManager.UserExsist(name, password))
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

        if (name == "")
        {
            ShowWarning("name is empty", 3);
        }
        else if (password == "")
        {
            ShowWarning("password is empty", 3);
        }
        else if (!acountManager.UserNameIsTaken(name))
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

    if (user.isAdmin)
    {
        clickEvent += std::bind(&Button::OnClick, &goToAdminButton, &mouseX, &mouseY);
    }

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

    clickEvent += std::bind(&Button::OnClick, &startTimedGameButton, &mouseX, &mouseY);
    clickEvent += std::bind(&Button::OnClick, &startSpeedRunGameButton, &mouseX, &mouseY);

    startTimedGameButton.event += [&]()
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

        currentGameMode = gmTimed;
        std::vector<Category>& cats = *catagories;
        quiz.StartQuiz(&cats, difficulty[difficultyIndex][0]);
        SetGame();
    };

    startSpeedRunGameButton.event += [&]()
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

        currentGameMode = gmspeedrun;
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
    currentHalfed = false;

    clickEvent += std::bind(&Button::OnClick, &SkipQuestionButton, &mouseX, &mouseY);
    clickEvent += std::bind(&Button::OnClick, &HalfQuestionsButton, &mouseX, &mouseY);

    if (currentGameMode == gmTimed)
    {
        startCurQtnTime = SDL_GetTicks() / 1000;

    }


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
        answerButtons.back().TextSetMaxWidth(450);


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
                mysql.AddNewScore(user.id, CurrentScore * 10, difficulty[difficultyIndex][0], elapsedTime, currentGameMode);
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

    //sepcial question
    if (quiz.GetQuestion(m_index)->value > 1)
    {
        QuestionText.NewColor({ 0,0,250 });
        QuestionText.NewText(quiz.GetQuestion(m_index)->title + " x2 points");
    }
    else
    {
        QuestionText.NewColor({ 0,0,0 });
        QuestionText.NewText(quiz.GetQuestion(m_index)->title);
    }
}

void SetGame()
{
    programState = game;
    clickEvent.Clear();
    scoreText.NewText("score:00");
    questionTotal = quiz.GetQuestionCount();
    startCurQtnTime = SDL_GetTicks() / 1000;

    QuestionText.NewText(quiz.GetQuestion(0)->title);
    NewQuestion(0);


    //elapsedTime
    SkipQuestionButton.event += [&]()
    {
        questionIndex++;
        printf("new questionIndex =  %d \n", questionIndex);
        if (questionIndex >= questionTotal)
        {
            //send score to database
            mysql.AddNewScore(user.id, CurrentScore * 10, difficulty[difficultyIndex][0], elapsedTime, currentGameMode);
            SetScoreboard();
        }
        else
        {
            NewQuestion(questionIndex);
        }
    };

    HalfQuestionsButton.event += [&]()
    {
        if (!currentHalfed)
        {
            currentHalfed = true;
            std::vector<Answer> ans = quiz.GetQuestion(questionIndex)->answers;

            bool atLeastOneTrue = false;
            int popCount = 0;
            int i = 0;

            while (popCount < (ans.size() / 2) + 1)
            {
                if (ans[i - popCount].isTrue && !atLeastOneTrue)
                {
                    atLeastOneTrue = true;
                }
                else
                {
                    answerButtons.erase(answerButtons.begin() + i - popCount);
                    ans.erase(ans.begin() + i - popCount);

                    popCount++;
                }
                i++;
            }

        }
    };



   startTime = SDL_GetTicks() / 1000;
    //difficulty
    if (gmTimed)
    {
        countdownTime = difficulty[difficultyIndex][1];
    }
}

void SetScoreboard()
{
    CurrentScore = 0;
    questionIndex = 0;
    questionTotal = 0;

    clickEvent.Clear();
    scores = mysql.GetScores(currentGameMode);

    int yPos = 200;
    for (const Score s : scores)
    {
        scoresScoreTexts.push_back(Text(std::to_string(s.score), 200, yPos, 50, gFont, { 10,10,10 }, gRenderer, left));
        scoresNameTexts.push_back(Text( s.userName , 300, yPos, 50, gFont, { 10,10,10 }, gRenderer, left));
        scoresQuestionCountTexts.push_back(Text(std::to_string(s.questionCount), 700, yPos, 50, gFont, { 10,10,10 }, gRenderer, left));
        scoresTimeTexts.push_back(Text(std::to_string(s.time), 900, yPos, 50, gFont, { 10,10,10 }, gRenderer, left));
        yPos += 50;
    }

    goToSetupFromScoreboardButton.event += std::bind(SetSetUp);
    clickEvent += std::bind(&Button::OnClick, &goToSetupFromScoreboardButton, &mouseX, &mouseY);

    pageTitle.NewText("scoreboard");
    programState = scoreboard;
}

void SetAdmin()
{
    pageTitle.NewText("admin");
    programState = admin;
    clickEvent.Clear();

    goToSetupFromAdminButton.event += std::bind(SetSetUp);
    clickEvent += std::bind(&Button::OnClick, &goToSetupFromAdminButton, &mouseX, &mouseY);

    std::vector<ISearchDialogable*>* items = adminSearchDialog.GetElements();
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
    startTimedGameButton.Render();
    startSpeedRunGameButton.Render();

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
    elapsedCurQtnTime = currentTime - startCurQtnTime;

    switch (currentGameMode)
    {
        case gmTimed:
        {
            if (countdownTime - elapsedTime < 1 || countdownCurQtnTime - elapsedCurQtnTime < 1)
            {
                SetScoreboard();
            }
            timeTotText.NewText("tot time:" + std::to_string(countdownTime - elapsedTime));
            timeCurText.NewText("time:" + std::to_string(countdownCurQtnTime - elapsedCurQtnTime));
            timeCurText.Render();
            break;
        }
        case gmspeedrun:
        {
            timeTotText.NewText("cur time:" + std::to_string(elapsedTime));
            break;
        }
        case gmNone:
        default:
        break;
    }

    timeTotText.Render();

    //catagoryButtons
    for (Button btn : answerButtons)
    {
        btn.Render();
    }

}

void UpdateScoreboard()
{
    pageTitle.Render();
    goToSetupFromScoreboardButton.Render();
    scoreboardScoreText.Render();
    scoreboardNameText.Render();
    scoreboardQuestionCountText.Render();
    scoreboardTimeText.Render();


    for (Panel p : scoreBoardPanels)
    {
        p.Render();
    }

    for (size_t i = 0; i < scores.size() - 1; i++)
    {
        scoresScoreTexts[i].Render();
        scoresNameTexts[i].Render();
        scoresQuestionCountTexts[i].Render();
        scoresTimeTexts[i].Render();
    }

}

void UpdateAdminPanel()
{
    switch (showAdminPanel)
    {
    case NewQstn:
    {
        adminSubSearchDialog.Render();
        adminTextInput.Render();
        adminAddQuestionTextInput.Render();
        AddFalseQuestionButton.Render();
        AddTrueQuestionButton.Render();
        adminCatagoryQuestionTextInput.Render();
        break;
    }
    case delQstn:
    {
        adminSubSearchDialog.Render();
        break;
    }
    case NewCat:
    {
        adminTextInput.Render();
        break;
    }
    case delCat:
    {
        adminSubSearchDialog.Render();
        break;
    }
    case admUser:
        adminSubSearchDialog.Render();
        break;
    case delUser:
    {
        adminSubSearchDialog.Render();
        break;
    }
    default:
        break;
    }
    executeAdminButton.Render();
}

void UpdateAdmin()
{
    AdminBackPanel.Render();
    adminPanelPageText.Render();
    UpdateAdminPanel();
    pageTitle.Render();
    goToSetupFromAdminButton.Render();
    adminSearchDialog.Render();
}

// main update funtion that will execude the
// update that corosponds to current page enum
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