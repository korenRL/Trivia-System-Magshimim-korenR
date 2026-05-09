#include "LoginManager.h"
#include <iostream>

LoginManager::LoginManager(SqliteDataBase* db) : m_database(db)
{
}

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
	if (!m_database || !isValidUsername(username))
	{
		return 0;
	}

	std::lock_guard<std::mutex> lock(m_usersMutex);

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
		return 1;
	}

	return 0;
}

int LoginManager::signup(const std::string& username, const std::string& password, const std::string& email)
{
	if (!m_database || !isValidUsername(username))
	{
		return 0;
	}

	std::lock_guard<std::mutex> lock(m_usersMutex);

	if (m_database->isUserExist(username))
	{
		return 0;
	}

	if (m_database->addUser(username, password, email))
	{
		return 1;
	}

	return 0;
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