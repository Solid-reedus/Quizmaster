#include "MySQL.h"

MySQL::MySQL()
{

}

MySQL::~MySQL()
{
    Free();

}

void MySQL::Free()
{
    delete con;
    delete driver;

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