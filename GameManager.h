#pragma once

#include "SqliteDataBase.h"
#include "RoomManager.h"
#include "Game.h"
#include <vector>

class GameManager
{
private:
	SqliteDataBase* m_database;
	std::vector<Game> m_games;

public:
	GameManager(SqliteDataBase* db);

	Game& createGame(const Room& room);
	Game& getGame();
	void deleteGame(unsigned int gameId);
};