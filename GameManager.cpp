#include "GameManager.h"
#include <iostream>

GameManager::GameManager(SqliteDataBase* db) : m_database(db)
{
}

void GameManager::startGame(const Room& room)
{
	m_questions = m_database->getQuestions(10);
	m_players.clear();

	for (const auto& username : room.players)
	{
		GameData data;
		data.correctAnswerCount = 0;
		data.wrongAnswerCount = 0;
		data.averageAnswerTime = 0;

		if (!m_questions.empty())
		{
			data.currentQuestion = m_questions[0];
		}

		m_players[username] = data;
	}
}

Question GameManager::getQuestionForUser(const std::string& username)
{
	if (m_players.find(username) == m_players.end())
	{
		return Question{};
	}

	return m_players[username].currentQuestion;
}

bool GameManager::submitAnswer(const std::string& username, unsigned int answerId, time_t answerTime)
{
	int i = 0;

	if (m_players.find(username) == m_players.end())
	{
		return false;
	}

	GameData& data = m_players[username];
	bool correct = (answerId == data.currentQuestion.correctAnswerId);

	if (correct)
	{
		data.correctAnswerCount++;
	}
	else
	{
		data.wrongAnswerCount++;
	}

	unsigned int totalAnswers = data.correctAnswerCount + data.wrongAnswerCount;
	data.averageAnswerTime = ((data.averageAnswerTime * (totalAnswers - 1)) + (unsigned int)answerTime / totalAnswers);

	for (i = 0; i < m_questions.size(); i++)
	{
		if (m_questions[i].question == data.currentQuestion.question)
		{
			if (i + 1 < m_questions.size())
			{
				data.currentQuestion = m_questions[i + 1];
			}

			break;
		}
	}

	return correct;
}

std::map<std::string, GameData> GameManager::getResults() const
{
	return m_players;
}

void GameManager::removePlayer(const std::string& username)
{
	if (m_players.find(username) != m_players.end())
	{
		m_players.erase(username);
	}
}

bool GameManager::isGameOver() const
{
	return m_players.empty();
}
