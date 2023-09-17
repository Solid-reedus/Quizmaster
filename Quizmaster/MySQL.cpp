#include "MySQL.h"


MySQL::MySQL()
{
    driver = NULL;
    con = NULL;

    if (ConnectToDb())
    {
        printf("connected to db \n \n");
    }
    else
    {
        printf("unable to connect to db \n \n");
    }

}

MySQL::~MySQL()
{
    Free();

}
void MySQL::Free()
{
    if (con != nullptr)
    {
        con->close();
        delete con;
        con = nullptr;
    }

    if (driver != nullptr)
    {
        sql::mysql::get_mysql_driver_instance()->threadEnd();
        driver = nullptr;
    }

}


bool MySQL::ConnectToDb()
{
    try 
    {
        // Create a MySQL driver object
        driver = sql::mysql::get_mysql_driver_instance();

        // Connect to the MySQL database
        con = driver->connect("tcp://127.0.0.1:3306", dbUser, dbPassword);

        // Connect to the specific database
        con->setSchema(dbName);

        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Error: " << e.what() << std::endl;
        return false;
    }

}


std::vector<Question> MySQL::GetQuestions(std::string m_category)
{

    std::vector<Question> result;
    if (!con)
    {
        printf("cannot get questions there isnt a connecttion \n");
        return result;
    }

    //std::vector<Question>* pQuestions = new std::vector<Question>;
    //Question(std::string m_title, std::vector<Answer> m_answers);

    try
    {
        // Create a statement
        sql::Statement* stmt;
        stmt = con->createStatement();

        // Execute a SQL query
        sql::ResultSet* res;

        std::ostringstream statement;
        statement << "SELECT questions.question_title, answer_name, answer_is_correct, answers.question_id, questions.category_id FROM answers JOIN questions ON answers.question_id = questions.question_id WHERE questions.category_id = " + m_category + " ORDER BY questions.question_id ASC;";
        //std::string statement = "SELECT questions.question_title,answer_name, answer_is_correct, answers.question_id, questions.category_id FROM answers JOIN questions ON answers.question_id = questions.question_id WHERE questions.category_id = " + m_category + " ORDER BY questions.question_id ASC;";
        res = stmt->executeQuery(statement.str());

        int colCount = res->getMetaData()->getColumnCount();
        int questionCount = -1;


        //get last index of pQuestions and add data

        //std::vector<Answer>* pAnswers;

        if (colCount > 0)
        {
            result.push_back(Question("", std::vector<Answer>()));

        }



        while (res->next()) 
        {

            //titlt
            std::string colTitle = res->getString(1);
            //name 
            std::string colName = res->getString(2);
            //is correct
            std::string colIsCorrect = res->getString(3);
            // question id
            std::string colCatId = res->getString(4);

            bool isCorrect = false;
            if (colIsCorrect == "1")
            {
                isCorrect = true;
            }

            result.back().AddAnswer(colName, isCorrect);
            if (std::stoi(colCatId) > questionCount)
            {
                questionCount = std::stoi(colCatId);
                result.push_back(Question(colTitle, std::vector<Answer>()));
            }

            //printf("columnNameNum1 = %s \n", columnNameNum1);
            std::cout << "\n";
            std::cout << "\n";
            std::cout << "columnNameNum1 = " << colTitle << "\n";
            std::cout << "columnNameNum1 = " << colName << "\n";
            std::cout << "columnNameNum1 = " << colIsCorrect << "\n";
            std::cout << "columnNameNum1 = " << colCatId << "\n";
            std::cout << "\n";
            std::cout << "\n";

            /*
            for (int i = 1; i <= colCount; ++i)
            {

                std::string columnName = res->getMetaData()->getColumnName(i);
                std::string columnValue = res->getString(i);
                
                if (columnName == "question_id" &&
                    std::stoi(columnValue) > questionCount)
                {
                    questionCount = std::stoi(columnValue);
                    printf("\n \n added new question nr %d \n \n", questionCount);

                }
                else if (true)
                {
                    //pQuestions->back().AddAnswer();
                }

                std::cout << columnName << ": " << columnValue << std::endl;
            }
            */
            std::cout << std::endl; // Separate rows with an empty line
        }

        //result = *pQuestions;
    }
    catch (const std::exception& e)
    {
        printf("Exception: %s\n", e.what());
    }

    return result;
}

void MySQL::MakeAcount(std::string m_name, std::string m_password)
{
    if (!con)
    {
        printf("cannot make a new acount there isnt a connecttion \n");
        return;
    }
}

User* MySQL::GetUser(std::string m_name, std::string m_password)
{
    if (!con)
    {
        printf("cannot get users there isnt a connecttion \n");
        return nullptr;
    }
}