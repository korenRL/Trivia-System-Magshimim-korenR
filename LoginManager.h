#pragma once

#include "SqliteDataBase.h"
#include <vector>
#include <string>
#include <mutex>

struct LoggedUser
{
	std::string username;
	LoggedUser(const std::string& name) : username(name) {}
};

class LoginManager
{
private:
	SqliteDataBase* m_database;
	std::vector<LoggedUser> m_loggedUsers;
	std::mutex m_usersMutex;

public:
	LoginManager(SqliteDataBase* db);

	int login(const std::string& username, const std::string& password);
	int signup(const std::string& username, const std::string& password, const std::string& email);
	void logout(const std::string& username);
};