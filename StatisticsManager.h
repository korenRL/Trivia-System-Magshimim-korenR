#pragma once

#include "SqliteDataBase.h"
#include <vector>
#include <string>

struct PlayerStatistics
{
	unsigned int gamesPlayed;
	unsigned int correctAnswers;
	unsigned int wrongAnswers;
	float avgAnswerTime;
};

class StatisticsManager
{
private:
	SqliteDataBase* m_database;

public:
	StatisticsManager(SqliteDataBase* db);
	
	PlayerStatistics getPlayerStatistics(const std::string& username);
};