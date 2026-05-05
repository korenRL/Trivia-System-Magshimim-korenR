#include "LoginManager.h"
#include <iostream>

LoginManager::LoginManager(SqliteDataBase* db)
    : m_database(db)
{
}

bool LoginManager::isUserLoggedIn(const std::string& username)
{
    for (const auto& user : m_loggedUsers)
    {
        if (user.username == username)
        {
            return true;
        }
    }
    return false;
}

int LoginManager::login(const std::string& username, const std::string& password)
{
    if (!m_database)
    {
        return 0;
    }

    if (username.empty() || password.empty())
    {
        return 0;
    }

    if (!m_database->isUserExist(username))
    {
        std::cout << "User does not exist: " << username << std::endl;
        return 0;
    }

    if (!m_database->isPasswordMatch(username, password))
    {
        std::cout << "Wrong password for " << username << std::endl;
        return 0;
    }

    if (isUserLoggedIn(username))
    {
        std::cout << username << " is already logged in" << std::endl;
        return 0;
    }

    m_loggedUsers.push_back(LoggedUser(username));
    std::cout << username << " logged in successfully" << std::endl;
    return 1;
}

int LoginManager::signup(const std::string& username, const std::string& password, const std::string& email)
{
    if (!m_database)
    {
        return 0;
    }

    if (username.empty() || password.empty() || email.empty())
    {
        return 0;
    }

    if (m_database->isUserExist(username))
    {
        std::cout << "User already exists: " << username << std::endl;
        return 0;
    }

    if (!m_database->addUser(username, password, email))
    {
        std::cout << "Signup failed for " << username << std::endl;
        return 0;
    }

    std::cout << username << " signed up successfully" << std::endl;
    return 1;
}