

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

// Basic validation ebfore cheking the database.
static bool isValidUsername(const std::string& username)
{
    if (username.empty())
    {
        return false;
    }
    return true;
}

int LoginManager::login(const std::string& username, const std::string& password)
{
    if (!m_database)
    {
        return 0;
    }

    if (!isValidUsername(username) || password.empty())
    {
        return 0;
    }

    std::lock_guard<std::mutex> lock(m_usersMutex);

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

    if (!isValidUsername(username) || password.empty() || email.empty())
    {
        return 0;
    }

    std::lock_guard<std::mutex> lock(m_usersMutex);

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

void LoginManager::logout(const std::string& username)
{
    std::lock_guard<std::mutex> lock(m_usersMutex);

    for (auto it = m_loggedUsers.begin(); it != m_loggedUsers.end(); ++it)
    {
        if (it->username == username)
        {
            m_loggedUsers.erase(it);
            break;
        }
    }
}