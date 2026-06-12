#include "Game.h"

Game::Game(const Room& room, std::vector<Question> questions, SqliteDataBase* db)
	: m_questions(questions), m_gameId(room.metadata.id), m_database(db)
{
	for (const auto& username : room.players)
	{
		GameData data;
		data.correctAnswerCount = 0;
		data.wrongAnswerCount = 0;
		data.averageAnswerTime = 0;
		data.questionIndex = 0;
		data.lastQuestionTime = 0;
		data.statsSaved = false;

		if (!m_questions.empty())
		{
			data.currentQuestion = m_questions[0];
		}

		m_players[username] = data;
	}
}

Question Game::getQuestionForUser(const std::string& username)
{
	if (m_players.find(username) == m_players.end())
	{
		return Question{};
	}

	GameData& data = m_players[username];

	if (data.questionIndex >= m_questions.size())
	{
		if (!data.statsSaved)
		{
			submitGameStatsToDB(username, data);
			data.statsSaved = true;
		}

		return Question{};
	}

	data.currentQuestion = m_questions[data.questionIndex];
	data.lastQuestionTime = std::time(NULL);

	return data.currentQuestion;
}

unsigned int Game::submitAnswer(const std::string& username, unsigned int answerId, time_t answerTime)
{
	if (m_players.find(username) == m_players.end())
	{
		return 0;
	}

	GameData& data = m_players[username];

	if (data.questionIndex >= m_questions.size())
	{
		if (!data.statsSaved)
		{
			submitGameStatsToDB(username, data);
			data.statsSaved = true;
		}

		return 0;
	}

	unsigned int correctId = m_questions[data.questionIndex].correctAnswerId;

	if (answerId == correctId)
	{
		data.correctAnswerCount++;
	}
	else
	{
		data.wrongAnswerCount++;
	}

	float elapsed = 0;
	if (data.lastQuestionTime != 0 && answerTime > data.lastQuestionTime)
	{
		elapsed = (float)(answerTime - data.lastQuestionTime);
	}

	unsigned int totalAnswers = data.correctAnswerCount + data.wrongAnswerCount;
	data.averageAnswerTime = ((data.averageAnswerTime * (totalAnswers - 1)) + elapsed) / totalAnswers;

	data.questionIndex++;

	if (data.questionIndex >= m_questions.size() && !data.statsSaved)
	{
		submitGameStatsToDB(username, data);
		data.statsSaved = true;
	}

	return correctId;
}

void Game::submitGameStatsToDB(const std::string& username, const GameData& data)
{
	if (m_database != nullptr)
	{
		m_database->submitGameStatistics(username, data);
	}
}

void Game::removePlayer(const std::string& username)
{
	if (m_players.find(username) == m_players.end())
	{
		return;
	}

	GameData& data = m_players[username];

	while (data.questionIndex < m_questions.size())
	{
		data.wrongAnswerCount++;
		data.questionIndex++;
	}

	if (!data.statsSaved)
	{
		submitGameStatsToDB(username, data);
		data.statsSaved = true;
	}
}

std::map<std::string, GameData> Game::getResults() const
{
	return m_players;
}

unsigned int Game::getGameId() const
{
	return m_gameId;
}

bool Game::isEmpty() const
{
	return m_players.empty();
}

bool Game::isFinished() const
{
	if (m_players.empty())
	{
		return true;
	}

	for (const auto& pair : m_players)
	{
		if (pair.second.questionIndex < m_questions.size())
		{
			return false;
		}
	}

	return true;
}