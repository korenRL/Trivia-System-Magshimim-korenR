//#include "LoginManager.h"
//#include <iostream>
//
//LoginManager::LoginManager(SqliteDataBase* db)
//    : m_database(db)
//{
//}
//
//<<<<<<< HEAD
//bool LoginManager::isUserLoggedIn(const std::string& username)
//{
//    for (const auto& user : m_loggedUsers)
//    {
//        if (user.username == username)
//        {
//            return true;
//        }
//    }
//    return false;
//=======
//static bool isValidUsername(const std::string& username)
//{
//	if (username.empty())
//	{
//		return false;
//	}
//	return true;
//>>>>>>> origin/develop
//}
//
//int LoginManager::login(const std::string& username, const std::string& password)
//{
//<<<<<<< HEAD
//    if (!m_database)
//    {
//        return 0;
//    }
//
//    if (username.empty() || password.empty())
//    {
//        return 0;
//    }
//
//    if (!m_database->isUserExist(username))
//    {
//        std::cout << "User does not exist: " << username << std::endl;
//        return 0;
//    }
//
//    if (!m_database->isPasswordMatch(username, password))
//    {
//        std::cout << "Wrong password for " << username << std::endl;
//        return 0;
//    }
//
//    if (isUserLoggedIn(username))
//    {
//        std::cout << username << " is already logged in" << std::endl;
//        return 0;
//    }
//
//    m_loggedUsers.push_back(LoggedUser(username));
//    std::cout << username << " logged in successfully" << std::endl;
//    return 1;
//=======
//	if (!m_database || !isValidUsername(username))
//	{
//		return 0;
//	}
//
//	std::lock_guard<std::mutex> lock(m_usersMutex);
//
//	if (m_database->isUserExist(username) && m_database->isPasswordMatch(username, password))
//	{
//		for (const auto& user : m_loggedUsers)
//		{
//			if (user.username == username)
//			{
//				return 0;
//			}
//		}
//
//		m_loggedUsers.push_back(LoggedUser(username));
//		return 1;
//	}
//
//	return 0;
//>>>>>>> origin/develop
//}
//
//int LoginManager::signup(const std::string& username, const std::string& password, const std::string& email)
//{
//<<<<<<< HEAD
//    if (!m_database)
//    {
//        return 0;
//    }
//
//    if (username.empty() || password.empty() || email.empty())
//    {
//        return 0;
//    }
//
//    if (m_database->isUserExist(username))
//    {
//        std::cout << "User already exists: " << username << std::endl;
//        return 0;
//    }
//
//    if (!m_database->addUser(username, password, email))
//    {
//        std::cout << "Signup failed for " << username << std::endl;
//        return 0;
//    }
//
//    std::cout << username << " signed up successfully" << std::endl;
//    return 1;
//=======
//	if (!m_database || !isValidUsername(username))
//	{
//		return 0;
//	}
//
//	std::lock_guard<std::mutex> lock(m_usersMutex);
//
//	if (m_database->isUserExist(username))
//	{
//		return 0;
//	}
//
//	if (m_database->addUser(username, password, email))
//	{
//		return 1;
//	}
//
//	return 0;
//}
//
//void LoginManager::logout(const std::string& username)
//{
//	std::lock_guard<std::mutex> lock(m_usersMutex);
//
//	for (auto it = m_loggedUsers.begin(); it != m_loggedUsers.end(); ++it)
//	{
//		if (it->username == username)
//		{
//			m_loggedUsers.erase(it);
//			break;
//		}
//	}
//>>>>>>> origin/develop
//}


#include "LoginManager.h"
#include <iostream>

LoginManager::LoginManager(SqliteDataBase* db)
    : m_database(db)
{
}

// Helper function from your branch
bool LoginManager::isUserLoggedIn(const std::string& username)
{
    // Note: We don't lock here because this function is called from inside 
    // login() which already holds the lock.
    for (const auto& user : m_loggedUsers)
    {
        if (user.username == username)
        {
            return true;
        }
    }
    return false;
}

// Helper function from develop branch
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

    // Critical: Lock before accessing database and logged users list
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

    // Critical: Lock before modifying database
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

// Logout function from develop branch
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