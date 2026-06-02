#pragma once

#include "SqliteDataBase.h"
#include "RoomManager.h"
#include <vector>
#include <map>
#include <string>
#include <ctime>
#include "structs.h"

struct GameData
{
	Question currentQuestion;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;
};

class GameManager
{
private:
	SqliteDataBase* m_database;
	std::map<std::string, GameData> m_players;
	std::vector<Question> m_questions;

public:
	GameManager(SqliteDataBase* db);

	void startGame(const Room& room);
	Question getQuestionForUser(const std::string& username);
	bool submitAnswer(const std::string& username, unsigned int answerId, time_t answerTime);
	std::map<std::string, GameData> getResults() const;
	void removePlayer(const std::string& username);
	bool isGameOver() const;
};

