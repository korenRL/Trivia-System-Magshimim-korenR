#include "LoginManager.h"
#include <iostream>

LoginManager::LoginManager(SqliteDataBase* db) : m_database(db)
{
}

int LoginManager::login(const std::string& username, const std::string& password)
{
	if (!m_database)
	{
		return 0;
	}

	if (m_database->isUserExist(username) && m_database->isPasswordMatch(username, password))
	{
		for (const auto& user : m_loggedUsers)
		{
			if (user.username == username)
			{
				return 0;
			}
		}

		m_loggedUsers.push_back(LoggedUser(username));
		std::cout << username << " logged in successfully" << std::endl;
		return 1;
	}

	std::cout << "Login failed for " << username << std::endl;
	return 0;
}

int LoginManager::signup(const std::string& username, const std::string& password, const std::string& email)
{
	if (!m_database)
	{
		return 0;
	}

	if (m_database->isUserExist(username))
	{
		std::cout << "User already exists: " << username << std::endl;
		return 0;
	}

	if (m_database->addUser(username, password, email))
	{
		m_loggedUsers.push_back(LoggedUser(username));
		std::cout << username << " signed up and logged in" << std::endl;
		return 1;
	}

	std::cout << "Signup failed for " << username << std::endl;
	return 0;
}