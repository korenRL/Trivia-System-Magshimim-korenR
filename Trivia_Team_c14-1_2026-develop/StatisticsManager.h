#pragma once

#include "SqliteDataBase.h"
#include "structs.h"
#include <vector>
#include <string>

class StatisticsManager
{
private:
	SqliteDataBase* m_database;

public:
	StatisticsManager(SqliteDataBase* db);

	PlayerStatistics getPlayerStatistics(const std::string& username);
	std::vector<std::string> getHighScores();
	std::vector<std::string> getPersonalStats(const std::string& username);
};