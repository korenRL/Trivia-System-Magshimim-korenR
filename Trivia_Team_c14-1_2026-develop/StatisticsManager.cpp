#include "StatisticsManager.h"

StatisticsManager::StatisticsManager(SqliteDataBase* db) : m_database(db)
{
}

PlayerStatistics StatisticsManager::getPlayerStatistics(const std::string& username)
{
	return m_database->getPlayerStatistics(username);
}

std::vector<std::string> StatisticsManager::getHighScores()
{
	std::vector<std::string> result;
	auto allStats = m_database->getAllStatistics();

	for (const auto& pair : allStats)
	{
		result.push_back(pair.first + ": " + std::to_string(pair.second.correctAnswers) + " correct answers");
	}

	return result;
}

std::vector<std::string> StatisticsManager::getPersonalStats(const std::string& username)
{
	std::vector<std::string> result;
	PlayerStatistics stats = getPlayerStatistics(username);

	result.push_back("Games Played: " + std::to_string(stats.gamesPlayed));
	result.push_back("Correct Answers: " + std::to_string(stats.correctAnswers));
	result.push_back("Wrong Answers: " + std::to_string(stats.wrongAnswers));
	result.push_back("Average Answer Time: " + std::to_string(stats.avgAnswerTime));

	return result;
}