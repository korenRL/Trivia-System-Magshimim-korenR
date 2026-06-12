#pragma once

#include "SqliteDataBase.h"
#include "RoomManager.h"
#include "structs.h"
#include <vector>
#include <map>
#include <string>
#include <ctime>

class Game
{
private:
	std::vector<Question> m_questions;
	std::map<std::string, GameData> m_players;
	unsigned int m_gameId;
	SqliteDataBase* m_database;

	void submitGameStatsToDB(const std::string& username, const GameData& data);

public:
	Game() = default;
	Game(const Room& room, std::vector<Question> questions, SqliteDataBase* db);

	Question getQuestionForUser(const std::string& username);
	unsigned int submitAnswer(const std::string& username, unsigned int answerId, time_t answerTime);
	void removePlayer(const std::string& username);
	std::map<std::string, GameData> getResults() const;
	unsigned int getGameId() const;
	bool isEmpty() const;
	bool isFinished() const;
};