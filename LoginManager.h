#pragma once

#include "SqliteDataBase.h"
#include <vector>
#include <string>

struct LoggedUser
{
    std::string username;

    LoggedUser(const std::string& name)
        : username(name)
    {
    }
};

class LoginManager
{
private:
    SqliteDataBase* m_database;
    std::vector<LoggedUser> m_loggedUsers;

    bool isUserLoggedIn(const std::string& username);

public:
    LoginManager(SqliteDataBase* db);

    int login(const std::string& username, const std::string& password);
    int signup(const std::string& username, const std::string& password, const std::string& email);
};