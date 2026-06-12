#pragma once

#include "sqlite3.h"
#include <string>
#include <vector>
#include "structs.h"
#include <utility>
#include <mutex>

class SqliteDataBase
{
public:
	SqliteDataBase(const std::string& dbName);
	~SqliteDataBase();

	bool addUser(const std::string& username, const std::string& password, const std::string& email);

	bool isUserExist(const std::string& username);
	bool isPasswordMatch(const std::string& username, const std::string& password);
	std::vector<Question> getQuestions(int amount);
	PlayerStatistics getPlayerStatistics(const std::string& username);
	std::vector<std::pair<std::string, PlayerStatistics>> getAllStatistics();
	int submitGameStatistics(const std::string& username, const GameData& data);

private:
	sqlite3* db;
	std::mutex m_dbMutex;
};