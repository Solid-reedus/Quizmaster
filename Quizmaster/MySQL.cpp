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

bool MySQL::HasEnoughQuestions(std::vector<Category>* m_categories, int m_count)
{
    if (!con)
    {
        printf("cannot get questions there isnt a connecttion \n");
        return false;
    }
    if (m_categories->empty())
    {
        printf("m_categories empty unable to check for questions \n");
        return false;
    }

    try
    {

        std::string categories = "";
        for (Category cat : *m_categories)
        {
            if (cat.isSelected)
            {
                categories = categories + " " + std::to_string(cat.id) + ",";
            }
        }
        categories.pop_back();



        // Create a statement
        sql::Statement* stmt;
        stmt = con->createStatement();
        // Execute a SQL query
        sql::ResultSet* res;
        std::ostringstream statement;
        statement << "SELECT question_id FROM questions WHERE category_id IN (" << categories << ")";
        res = stmt->executeQuery(statement.str());

        int count = 0;

        while (res->next())
        {
            count++;
        }

        if (count >= m_count)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    catch (const std::exception& e)
    {
        printf("Exception: %s\n", e.what());
        return false;
    }

}


std::vector<Question> MySQL::GetQuestions(std::vector<Category>* m_categories, int m_amount)
{
    std::vector<Question> result;
    if (!con)
    {
        printf("cannot get questions there isnt a connecttion \n");
        return result;
    }
    if (m_categories->empty())
    {
        printf("m_categories empty unable to return anything \n");
        return result;
    }

    try
    {

        std::string categories = "";
        for (Category cat : *m_categories)
        {
            if (cat.isSelected)
            {
                categories = categories + " " + std::to_string(cat.id) + ",";
            }
        }
        categories.pop_back();
        
        // Create a statement
        sql::Statement* stmt;
        stmt = con->createStatement();
        // Execute a SQL query
        sql::ResultSet* res;
        //std::ostringstream statement;

        /*
                SELECT
            questions.question_title,
            answers.answer_name,
            answers.answer_is_correct,
            questions.question_id,
            questions.category_id
        FROM
            (
                SELECT
                    question_id
                FROM
                    questions
                WHERE
                    category_id IN (1, 3)
                ORDER BY
                    RAND()
                LIMIT 10
            ) AS random_questions
        JOIN
            questions ON random_questions.question_id = questions.question_id
        JOIN
            answers ON questions.question_id = answers.question_id;
        
        */

        std::ostringstream statement;
        statement << "SELECT questions.question_title, answers.answer_name, answers.answer_is_correct, questions.question_id, questions.category_id "
            << "FROM (SELECT question_id FROM questions WHERE category_id IN (" << categories << ") ORDER BY RAND() LIMIT " << std::to_string(m_amount) << ") AS random_questions "
            << "JOIN questions ON random_questions.question_id = questions.question_id "
            << "JOIN answers ON questions.question_id = answers.question_id "
            << "ORDER BY questions.question_id;";

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

std::vector<Category>* MySQL::GetCategories()
{

    if (!con)
    {
        printf("cannot get questions there isnt a connecttion \n");
        return nullptr;
    }

    try
    {
        // Create a statement
        sql::Statement* stmt;
        stmt = con->createStatement();
        // Execute a SQL query
        sql::ResultSet* res;
        std::ostringstream statement;
        statement << "SELECT * FROM categories";
        res = stmt->executeQuery(statement.str());

        std::vector<Category>* result = new std::vector<Category>();

        while (res->next())
        {
            int id = res->getInt("category_id");
            std::string name = res->getString("category_name");
            result->push_back(Category(id, name));
        }
        return result;
    }
    catch (const std::exception& e)
    {
        printf("Exception: %s\n", e.what());
        return nullptr;
    }

}

bool MySQL::UserNameIsTaken(std::string m_name)
{
    if (!con)
    {
        printf("cannot get questions there isnt a connecttion \n");
        return false;
    }

    if (m_name == "")
    {
        printf("name isnt filled in \n");
    }

    try
    {
        // Create a statement
        sql::Statement* stmt;
        stmt = con->createStatement();

        // Execute a SQL query
        sql::ResultSet* res;

        std::ostringstream statement;

        statement << "SELECT user_name FROM users WHERE user_name = '" << m_name << "';";

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

        std::string encryptedPassword = encryptDecrypt(m_password, 1);

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
        std::string password = encryptDecrypt(m_password, 1);

        sql::Statement* stmt;
        stmt = con->createStatement();
        // Execute a SQL query
        sql::ResultSet* res;
        std::ostringstream statement;
        statement << "SELECT user_name, user_score FROM users WHERE user_name = '" << m_name << "' AND user_password = '" << password << "';";
        //statement << "SELECT user_name,user_score FROM users WHERE user_name = "+ m_name +" AND user_password = "+ password +";";
        res = stmt->executeQuery(statement.str());

        User* user = nullptr;

        if (res->next())
        {
            //res->getString("name_column_name");
            user = new User(res->getString("user_name"), res->getInt("user_score"));
        }
        else
        {
            printf("result for user is empty");
        }

        delete res;
        delete stmt;

        return user;
        //SELECT * FROM users WHERE user_name = "jeff" AND user_password = "rcuuyqtf";
    }
    catch (const std::exception& e)
    {
        std::cerr << "SQL Error: " << e.what() << std::endl;
    }
}

std::string MySQL::encryptDecrypt(const std::string& text, char key)
{
    std::string result = text;
    for (char& character : result) {
        character ^= key;
    }
    return result;
}

bool MySQL::UserExsist(std::string m_name, std::string m_password)
{

    if (!con)
    {
        printf("cannot get questions there isnt a connecttion \n");
        return false;
    }

    if (m_name == "" || m_password == "")
    {
        printf("name and password arent filled in \n");
    }

    try
    {
        // Create a statement
        sql::Statement* stmt;
        stmt = con->createStatement();

        // Execute a SQL query
        sql::ResultSet* res;

        std::ostringstream statement;

        std::string decriptedPassword = encryptDecrypt(m_password, 1);

        statement << "SELECT user_name, user_password FROM users WHERE user_name = '" << m_name << "' AND user_password = '" << decriptedPassword << "';";

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


