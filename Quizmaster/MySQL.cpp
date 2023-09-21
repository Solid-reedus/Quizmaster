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

    //MakeAcount("jeff", "password");

}

MySQL::~MySQL()
{
    con->close();
    delete con;
    sql::mysql::get_mysql_driver_instance()->threadEnd();
    driver = nullptr;
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

    try
    {
        // Create a statement
        sql::Statement* stmt;
        stmt = con->createStatement();

        // Execute a SQL query
        sql::ResultSet* res;

        std::ostringstream statement;
        statement << "SELECT DISTINCT  questions.question_title, answer_name, answer_is_correct, answers.question_id, questions.category_id FROM answers JOIN questions ON answers.question_id = questions.question_id WHERE questions.category_id = " + m_category + " ORDER BY questions.question_id ASC;";
        //std::string statement = "SELECT questions.question_title,answer_name, answer_is_correct, answers.question_id, questions.category_id FROM answers JOIN questions ON answers.question_id = questions.question_id WHERE questions.category_id = " + m_category + " ORDER BY questions.question_id ASC;";
        res = stmt->executeQuery(statement.str());

        int colCount = res->getMetaData()->getColumnCount();
        int questionCount = -1;

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

            if (std::stoi(colCatId) > questionCount)
            {
                questionCount = std::stoi(colCatId);
                result.push_back(Question(colTitle, std::vector<Answer>()));
            }

            bool isCorrect = false;
            if (colIsCorrect == "1")
            {
                isCorrect = true;
            }

            result.back().AddAnswer(colName, isCorrect);


            for (int i = 1; i <= colCount; ++i) 
            {
                std::string colName = res->getMetaData()->getColumnName(i);
                std::string colVal = res->getString(i);

                std::cout << colName << " : " << colVal << " \n";
            }
            printf("\n \n");
        }
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

    try
    {
        std::string insertQuery = "INSERT INTO users (user_name, user_password, user_is_admin) VALUES (?, ?, ?)";
        sql::PreparedStatement* pstmt = con->prepareStatement(insertQuery);

        std::string encryptedPassword = encryptCaesarCipher(m_password, 2);

        pstmt->setString(1, m_name);
        pstmt->setString(2, encryptedPassword);
        pstmt->setString(3, "0");

        pstmt->executeUpdate();
        delete pstmt;
    }
    catch (sql::SQLException& e) 
    {
        std::cerr << "SQL Error: " << e.what() << std::endl;
    }
}

User* MySQL::GetUser(std::string m_name, std::string m_password)
{
    if (!con)
    {
        printf("cannot get users there isnt a connecttion \n");
        return nullptr;
    }

    try
    {

    }
    catch (const std::exception&)
    {

    }
}

std::string MySQL::encryptCaesarCipher(const std::string& m_plaintext, int m_shift)
{
    std::string encryptedText = "";

    for (char c : m_plaintext)
    {
        if (isalpha(c)) 
        {
            char base = (isupper(c)) ? 'A' : 'a';
            char shifted = static_cast<char>((c - base + m_shift) % 26 + base);

            if (shifted < base) 
            {
                // Wrap around if shifted character is less than the base
                shifted += 26;
            }

            encryptedText += shifted;
        }
        else 
        {
            encryptedText += c; // Non-alphabet characters remain unchanged
        }
    }

    return encryptedText;
}

bool MySQL::UserExsist(std::string m_name, std::string m_password)
{

    if (!con)
    {
        printf("cannot get questions there isnt a connecttion \n");
        return false;
    }

    try
    {
        // Create a statement
        sql::Statement* stmt;
        stmt = con->createStatement();

        // Execute a SQL query
        sql::ResultSet* res;

        std::ostringstream statement;

        std::string decriptedPassword = encryptCaesarCipher(m_password, -2);

        statement << "SELECT user_name, user_password FROM `users` WHERE user_name = " + m_name + " AND user_password = " + decriptedPassword + ";";

        res = stmt->executeQuery(statement.str());

        if (!res->next())
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "SQL Error: " << e.what() << std::endl;
    }
    return false;

}


