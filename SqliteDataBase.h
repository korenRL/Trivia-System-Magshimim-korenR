#pragma once

#include "sqlite3.h"
#include <string>
#include <vector>
#include "structs.h"

class SqliteDataBase
{
public:
	SqliteDataBase(const std::string& dbName);
	~SqliteDataBase();

	bool addUser(const std::string& username, const std::string& password, const std::string& email);

	bool isUserExist(const std::string& username);
	bool isPasswordMatch(const std::string& username, const std::string& password);
	std::vector<Question> getQuestions(int amount);

private:
	sqlite3* db;

};