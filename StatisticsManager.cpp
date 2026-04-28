#include "StatisticsManager.h"

StatisticsManager::StatisticsManager(SqliteDataBase* db) : m_database(db)
{
}

PlayerStatistics StatisticsManager::getPlayerStatistics(const std::string& username)
{
	PlayerStatistics stats{ 0, 0, 0, 0.0f };
	return stats;
}