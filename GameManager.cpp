#include "GameManager.h"

GameManager::GameManager(SqliteDataBase* db) : m_database(db)
{
}

Game& GameManager::createGame(const Room& room)
{
	std::vector<Question> questions = m_database->getQuestions(room.metadata.numOfQuestionsInGame);
	m_games.push_back(Game(room, questions, m_database));
	return m_games.back();
}

Game& GameManager::getGame()
{
	return m_games.back();
}

void GameManager::deleteGame(unsigned int gameId)
{
	for (auto it = m_games.begin(); it != m_games.end(); ++it)
	{
		if (it->getGameId() == gameId)
		{
			m_games.erase(it);
			return;
		}
	}
}