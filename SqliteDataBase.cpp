#include "SqliteDataBase.h"
#include <iostream>

SqliteDataBase::SqliteDataBase(const std::string& dbName)
{
	int res = sqlite3_open(dbName.c_str(), &db);
	if (res != SQLITE_OK)
	{
		std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		db = nullptr;
		return;
	}

	const char* sqlUsers = "CREATE TABLE IF NOT EXISTS users (username TEXT PRIMARY KEY, password TEXT NOT NULL, email TEXT NOT NULL);";

	char* errMsg = nullptr;
	res = sqlite3_exec(db, sqlUsers, nullptr, nullptr, &errMsg);
	if (res != SQLITE_OK)
	{
		std::cerr << "SQL error (users): " << errMsg << std::endl;
		sqlite3_free(errMsg);
	}

	const char* sqlStats = "CREATE TABLE IF NOT EXISTS statistics ("
		"username TEXT PRIMARY KEY, "
		"games_played INTEGER DEFAULT 0, "
		"total_correct_answers INTEGER DEFAULT 0, "
		"total_answers INTEGER DEFAULT 0, "
		"average_answer_time REAL DEFAULT 0.0, "
		"FOREIGN KEY(username) REFERENCES users(username));";

	res = sqlite3_exec(db, sqlStats, nullptr, nullptr, &errMsg);
	if (res != SQLITE_OK)
	{
		std::cerr << "SQL error (statistics): " << errMsg << std::endl;
		sqlite3_free(errMsg);
	}

	const char* sqlQuestions = "CREATE TABLE IF NOT EXISTS QUESTIONS ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"question TEXT NOT NULL, "
		"correct_answer TEXT NOT NULL, "
		"incorrect_answer_1 TEXT NOT NULL, "
		"incorrect_answer_2 TEXT NOT NULL, "
		"incorrect_answer_3 TEXT NOT NULL);";

	res = sqlite3_exec(db, sqlQuestions, nullptr, nullptr, &errMsg);
	if (res != SQLITE_OK)
	{
		std::cerr << "SQL error (questions): " << errMsg << std::endl;
		sqlite3_free(errMsg);
	}
	else
	{
		std::cout << "Database opened and all tables are ready." << std::endl;
	}
}
SqliteDataBase::~SqliteDataBase()
{
	if (db)
	{
		sqlite3_close(db);
	}
}

bool SqliteDataBase::addUser(const std::string& username, const std::string& password, const std::string& email)
{
	if (!db)
	{
		return false;
	}

	std::string sql = "INSERT INTO users (username, password, email) VALUES ('" + username + "', '" + password + "', '" + email + "');";

	char* errMsg = nullptr;
	int res = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
	if (res != SQLITE_OK)
	{
		std::cerr << "Add user failed: " << errMsg << std::endl;
		sqlite3_free(errMsg);
		return false;
	}

	return true;
}

bool SqliteDataBase::isUserExist(const std::string& username)
{
	if (!db)
	{
		return false;
	}

	std::string sql = "SELECT COUNT(*) FROM users WHERE username = '" + username + "';";
	int count = 0;

	auto callback = [](void* data, int argc, char** argv, char** colName) -> int
		{
			if (argc > 0 && argv[0])
			{
				*(int*)data = std::stoi(argv[0]);
			}
			return 0;
		};

	char* errMsg = nullptr;
	int res = sqlite3_exec(db, sql.c_str(), callback, &count, &errMsg);

	if (res != SQLITE_OK)
	{
		std::cerr << "isUserExist error: " << errMsg << std::endl;
		sqlite3_free(errMsg);
		return false;
	}
	
	if (count > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool SqliteDataBase::isPasswordMatch(const std::string& username, const std::string& password)
{
	if (!db)
	{
		return false;
	}

	std::string sql = "SELECT password FROM users WHERE username = '" + username + "';";
	std::string dbPass;

	auto callback = [](void* data, int argc, char** argv, char** colName) -> int
		{
			if (argc > 0 && argv[0])
			{
				*(std::string*)data = argv[0];
			}

			return 0;
		};

	char* errMsg = nullptr;
	int res = sqlite3_exec(db, sql.c_str(), callback, &dbPass, &errMsg);

	if (res != SQLITE_OK)
	{
		std::cerr << "isPasswordMatch error: " << errMsg << std::endl;
		sqlite3_free(errMsg);
		return false;
	}

	if (dbPass == password)
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::vector<Question> SqliteDataBase::getQuestions(int amount)
{
	std::vector<Question> questions;
	if (!db)
	{
		return questions;
	}

	std::string sql = "SELECT question, correct_answer, incorrect_answer_1, incorrect_answer_2, incorrect_answer_3 FROM QUESTIONS ORDER BY RANDOM() LIMIT " + std::to_string(amount) + ";";

	auto callback = [](void* data, int argc, char** argv, char** colName) -> int
		{
			if (argc >= 5)
			{
				Question q;
				q.question = argv[0];
				q.possibleAnswers.push_back(argv[1]);
				q.possibleAnswers.push_back(argv[2]);
				q.possibleAnswers.push_back(argv[3]);
				q.possibleAnswers.push_back(argv[4]);
				q.correctAnswerId = 0;
				((std::vector<Question>*)data)->push_back(q);
			}
			return 0;
		};

	char* errMsg = nullptr;
	int res = sqlite3_exec(db, sql.c_str(), callback, &questions, &errMsg);
	if(res != SQLITE_OK)
	{
		std::cerr << "getQuestions error: " << errMsg << std::endl;
		sqlite3_free(errMsg);
	}

	return questions;
}
